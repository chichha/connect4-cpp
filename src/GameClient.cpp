#include "GameClient.h"
#include <iostream>

GameClient::GameClient()
    : playerRole_(' '),
      state_(ClientState::DISCONNECTED),
      running_(false) {
}

GameClient::~GameClient() {
    disconnect();
}

bool GameClient::connect(const std::string& serverIP, int port) {
    if (state_ != ClientState::DISCONNECTED) {
        setError("Client already connected or connecting");
        return false;
    }
    
    state_ = ClientState::CONNECTING;
    setStatus("Connecting to " + serverIP + ":" + std::to_string(port));
    
    if (!socket_.initialize()) {
        setError("Failed to initialize network: " + socket_.getLastError());
        state_ = ClientState::ERROR;
        return false;
    }
    
    if (!socket_.connectToServer(serverIP, port)) {
        setError("Failed to connect to server: " + socket_.getLastError());
        socket_.cleanup();
        state_ = ClientState::ERROR;
        return false;
    }
    
    state_ = ClientState::CONNECTED;
    setStatus("Connected to server");
    
    // Send CONNECT message
    std::string connectMsg = NetworkProtocol::createConnectMessage("Player");
    socket_.send(connectMsg.c_str(), connectMsg.length());
    
    // Start receive thread
    running_ = true;
    receiveThread_ = std::thread(&GameClient::receiveLoopThread, this);
    
    return true;
}

void GameClient::disconnect() {
    if (state_ != ClientState::DISCONNECTED) {
        // Send disconnect message
        if (socket_.isConnected()) {
            std::string disconnectMsg = NetworkProtocol::createDisconnectMessage();
            socket_.send(disconnectMsg.c_str(), disconnectMsg.length());
        }
        
        running_ = false;
        
        if (receiveThread_.joinable()) {
            receiveThread_.join();
        }
        
        socket_.cleanup();
        state_ = ClientState::DISCONNECTED;
        playerRole_ = ' ';
        setStatus("Disconnected");
    }
}

ClientState GameClient::getState() const {
    return state_;
}

bool GameClient::isConnected() const {
    return state_ == ClientState::CONNECTED || state_ == ClientState::IN_GAME;
}

char GameClient::getPlayerRole() const {
    return playerRole_;
}

bool GameClient::sendMove(int column) {
    if (state_ != ClientState::IN_GAME) {
        setError("Not in game");
        return false;
    }
    
    if (!game_.validateMove(column, playerRole_)) {
        setError("Invalid move");
        return false;
    }
    
    std::string moveMsg = NetworkProtocol::createMoveMessage(column);
    int sent = socket_.send(moveMsg.c_str(), moveMsg.length());
    
    return sent == static_cast<int>(moveMsg.length());
}

bool GameClient::requestReset() {
    if (!isConnected()) {
        setError("Not connected");
        return false;
    }
    
    std::string resetMsg = NetworkProtocol::createResetMessage();
    int sent = socket_.send(resetMsg.c_str(), resetMsg.length());
    
    return sent == static_cast<int>(resetMsg.length());
}

const Game& GameClient::getGame() const {
    return game_;
}

std::string GameClient::getStatusMessage() const {
    std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(stateMutex_));
    return statusMessage_;
}

std::string GameClient::getLastError() const {
    return lastError_;
}

void GameClient::receiveLoopThread() {
    char buffer[NetworkManager::BUFFER_SIZE];
    
    while (running_) {
        int received = socket_.receive(buffer, sizeof(buffer) - 1);
        
        if (received > 0) {
            buffer[received] = '\0';
            receiveBuffer_ += buffer;
            
            auto messages = NetworkProtocol::extractMessages(receiveBuffer_);
            for (const auto& msgStr : messages) {
                NetworkMessage msg = NetworkProtocol::deserialize(msgStr);
                processMessage(msg);
            }
        } else if (received == 0) {
            // Connection closed
            setStatus("Server disconnected");
            running_ = false;
            break;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void GameClient::processMessage(const NetworkMessage& msg) {
    switch (msg.type) {
        case MessageType::ACCEPT: {
            playerRole_ = NetworkProtocol::parsePlayerRole(msg);
            state_ = ClientState::IN_GAME;
            game_.setGameMode(GameMode::NETWORK_CLIENT);
            setStatus("Joined game as Player " + std::string(1, playerRole_));
            break;
        }
        case MessageType::REJECT: {
            std::string reason = NetworkProtocol::parseRejectReason(msg);
            setError("Connection rejected: " + reason);
            state_ = ClientState::ERROR;
            break;
        }
        case MessageType::GAME_STATE: {
            std::string boardState;
            char currentPlayer;
            std::string gameStatus;
            
            if (NetworkProtocol::parseGameState(msg, boardState, currentPlayer, gameStatus)) {
                game_.deserializeGameState(boardState);
                setStatus(gameStatus == "OVER" ? "Game Over" : 
                         (currentPlayer == playerRole_ ? "Your turn" : "Opponent's turn"));
            }
            break;
        }
        case MessageType::WIN: {
            char winner = NetworkProtocol::parseWinner(msg);
            setStatus(winner == playerRole_ ? "You win!" : "You lose!");
            break;
        }
        case MessageType::DRAW: {
            setStatus("Game ended in a draw!");
            break;
        }
        case MessageType::PONG: {
            // Heartbeat response
            break;
        }
        case MessageType::DISCONNECT: {
            setStatus("Opponent disconnected");
            break;
        }
        default:
            break;
    }
}

void GameClient::setStatus(const std::string& message) {
    std::lock_guard<std::mutex> lock(stateMutex_);
    statusMessage_ = message;
}

void GameClient::setError(const std::string& error) {
    lastError_ = error;
    if (state_ != ClientState::ERROR) {
        state_ = ClientState::ERROR;
    }
}
