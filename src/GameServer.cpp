#include "GameServer.h"
#include <iostream>

GameServer::GameServer()
    : client1Socket_(INVALID_SOCKET_VALUE),
      client2Socket_(INVALID_SOCKET_VALUE),
      player1Char_('X'),
      player2Char_('O'),
      state_(ServerState::IDLE),
      port_(0),
      running_(false),
      connectedPlayers_(0) {
}

GameServer::~GameServer() {
    stop();
}

bool GameServer::start(int port) {
    if (running_) {
        setError("Server already running");
        return false;
    }
    
    port_ = port;
    
    if (!serverSocket_.initialize()) {
        setError("Failed to initialize network: " + serverSocket_.getLastError());
        return false;
    }
    
    if (!serverSocket_.createServerSocket(port)) {
        setError("Failed to create server socket: " + serverSocket_.getLastError());
        serverSocket_.cleanup();
        return false;
    }
    
    if (!serverSocket_.listen()) {
        setError("Failed to listen: " + serverSocket_.getLastError());
        serverSocket_.cleanup();
        return false;
    }
    
    running_ = true;
    state_ = ServerState::WAITING_FOR_PLAYERS;
    setStatus("Server started on port " + std::to_string(port));
    
    // Start accept thread
    acceptThread_ = std::thread(&GameServer::acceptConnectionsThread, this);
    
    return true;
}

void GameServer::stop() {
    running_ = false;
    
    // Close client sockets
    if (client1Socket_ != INVALID_SOCKET_VALUE) {
#ifdef _WIN32
        closesocket(client1Socket_);
#else
        close(client1Socket_);
#endif
        client1Socket_ = INVALID_SOCKET_VALUE;
    }
    
    if (client2Socket_ != INVALID_SOCKET_VALUE) {
#ifdef _WIN32
        closesocket(client2Socket_);
#else
        close(client2Socket_);
#endif
        client2Socket_ = INVALID_SOCKET_VALUE;
    }
    
    serverSocket_.cleanup();
    
    if (acceptThread_.joinable()) {
        acceptThread_.join();
    }
    
    if (gameThread_.joinable()) {
        gameThread_.join();
    }
    
    state_ = ServerState::IDLE;
    connectedPlayers_ = 0;
}

ServerState GameServer::getState() const {
    return state_;
}

std::string GameServer::getIPAddress() const {
    return NetworkManager::getLocalIPAddress();
}

int GameServer::getPort() const {
    return port_;
}

int GameServer::getConnectedPlayerCount() const {
    return connectedPlayers_;
}

bool GameServer::isGameStarted() const {
    return state_ == ServerState::GAME_IN_PROGRESS;
}

const Game& GameServer::getGame() const {
    return game_;
}

std::string GameServer::getStatusMessage() const {
    std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(stateMutex_));
    return statusMessage_;
}

std::string GameServer::getLastError() const {
    return lastError_;
}

void GameServer::acceptConnectionsThread() {
    while (running_ && connectedPlayers_ < 2) {
        // Set socket to non-blocking for timeout
        serverSocket_.setNonBlocking(true);
        
        SocketHandle clientSocket = serverSocket_.acceptConnection();
        
        if (clientSocket != INVALID_SOCKET_VALUE) {
            connectedPlayers_++;
            
            if (connectedPlayers_ == 1) {
                client1Socket_ = clientSocket;
                setStatus("Player 1 connected");
                
                // Send ACCEPT message with player role
                std::string acceptMsg = NetworkProtocol::createAcceptMessage(player1Char_);
                sendData(clientSocket, acceptMsg.c_str(), acceptMsg.length());
            } else if (connectedPlayers_ == 2) {
                client2Socket_ = clientSocket;
                setStatus("Player 2 connected - Game starting!");
                
                // Send ACCEPT message with player role
                std::string acceptMsg = NetworkProtocol::createAcceptMessage(player2Char_);
                sendData(clientSocket, acceptMsg.c_str(), acceptMsg.length());
                
                // Start game
                state_ = ServerState::GAME_IN_PROGRESS;
                game_.setGameMode(GameMode::NETWORK_HOST);
                
                // Broadcast initial game state
                broadcastGameState();
                
                // Start game loop thread
                gameThread_ = std::thread(&GameServer::gameLoopThread, this);
            }
        }
        
        // Small delay to avoid busy waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void GameServer::gameLoopThread() {
    char buffer[NetworkManager::BUFFER_SIZE];
    
    while (running_ && state_ == ServerState::GAME_IN_PROGRESS) {
        // Process messages from player 1
        if (client1Socket_ != INVALID_SOCKET_VALUE) {
            int received = receiveData(client1Socket_, buffer, sizeof(buffer) - 1);
            if (received > 0) {
                buffer[received] = '\0';
                receiveBuffer1_ += buffer;
                
                auto messages = NetworkProtocol::extractMessages(receiveBuffer1_);
                for (const auto& msgStr : messages) {
                    NetworkMessage msg = NetworkProtocol::deserialize(msgStr);
                    processMessage(msg, 1);
                }
            }
        }
        
        // Process messages from player 2
        if (client2Socket_ != INVALID_SOCKET_VALUE) {
            int received = receiveData(client2Socket_, buffer, sizeof(buffer) - 1);
            if (received > 0) {
                buffer[received] = '\0';
                receiveBuffer2_ += buffer;
                
                auto messages = NetworkProtocol::extractMessages(receiveBuffer2_);
                for (const auto& msgStr : messages) {
                    NetworkMessage msg = NetworkProtocol::deserialize(msgStr);
                    processMessage(msg, 2);
                }
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void GameServer::processMessage(const NetworkMessage& msg, int playerNum) {
    char playerChar = (playerNum == 1) ? player1Char_ : player2Char_;
    
    switch (msg.type) {
        case MessageType::MOVE: {
            int column = NetworkProtocol::parseMoveColumn(msg);
            if (game_.validateMove(column, playerChar)) {
                if (game_.makeMove(column)) {
                    broadcastGameState();
                    
                    // Check for game over
                    if (game_.isGameOver()) {
                        char winner = game_.getWinner();
                        if (winner != ' ') {
                            std::string winMsg = NetworkProtocol::createWinMessage(winner);
                            sendToClient(client1Socket_, winMsg);
                            sendToClient(client2Socket_, winMsg);
                        } else {
                            std::string drawMsg = NetworkProtocol::createDrawMessage();
                            sendToClient(client1Socket_, drawMsg);
                            sendToClient(client2Socket_, drawMsg);
                        }
                    }
                }
            }
            break;
        }
        case MessageType::RESET: {
            game_.reset();
            broadcastGameState();
            break;
        }
        case MessageType::PING: {
            SocketHandle socket = (playerNum == 1) ? client1Socket_ : client2Socket_;
            std::string pongMsg = NetworkProtocol::createPongMessage();
            sendToClient(socket, pongMsg);
            break;
        }
        default:
            break;
    }
}

void GameServer::broadcastGameState() {
    std::string boardState = game_.serializeGameState();
    char currentPlayer = game_.getCurrentPlayer();
    std::string gameStatus = game_.isGameOver() ? "OVER" : "PLAYING";
    
    std::string stateMsg = NetworkProtocol::createGameStateMessage(boardState, currentPlayer, gameStatus);
    
    sendToClient(client1Socket_, stateMsg);
    sendToClient(client2Socket_, stateMsg);
}

void GameServer::sendToClient(SocketHandle socket, const std::string& message) {
    if (socket != INVALID_SOCKET_VALUE) {
        sendData(socket, message.c_str(), message.length());
    }
}

void GameServer::setStatus(const std::string& message) {
    std::lock_guard<std::mutex> lock(stateMutex_);
    statusMessage_ = message;
}

void GameServer::setError(const std::string& error) {
    lastError_ = error;
    state_ = ServerState::ERROR;
}

bool GameServer::sendData(SocketHandle socket, const char* data, int length) {
    int totalSent = 0;
    while (totalSent < length) {
#ifdef _WIN32
        int sent = ::send(socket, data + totalSent, length - totalSent, 0);
#else
        ssize_t sent = ::send(socket, data + totalSent, length - totalSent, 0);
#endif
        if (sent <= 0) {
            return false;  // Error or connection closed
        }
        totalSent += sent;
    }
    return true;
}

int GameServer::receiveData(SocketHandle socket, char* buffer, int bufferSize) {
    // Set non-blocking
#ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(socket, FIONBIO, &mode);
    int received = recv(socket, buffer, bufferSize, 0);
    mode = 0;
    ioctlsocket(socket, FIONBIO, &mode);
#else
    int flags = fcntl(socket, F_GETFL, 0);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
    ssize_t received = recv(socket, buffer, bufferSize, 0);
    fcntl(socket, F_SETFL, flags);
#endif
    
    if (received < 0) {
#ifdef _WIN32
        if (WSAGetLastError() == WSAEWOULDBLOCK) return 0;
#else
        if (errno == EWOULDBLOCK || errno == EAGAIN) return 0;
#endif
        return -1;
    }
    
    return received;
}
