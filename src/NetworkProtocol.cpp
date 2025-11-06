#include "NetworkProtocol.h"
#include <sstream>

std::string NetworkProtocol::serialize(MessageType type, const std::string& payload) {
    std::ostringstream oss;
    oss << messageTypeToString(type);
    if (!payload.empty()) {
        oss << ":" << payload;
    }
    oss << MESSAGE_DELIMITER;
    return oss.str();
}

NetworkMessage NetworkProtocol::deserialize(const std::string& message) {
    NetworkMessage msg;
    
    if (message.empty() || message.back() != MESSAGE_DELIMITER) {
        return msg; // Invalid message
    }
    
    // Remove delimiter
    std::string cleanMsg = message.substr(0, message.length() - 1);
    
    // Find separator between type and payload
    size_t separatorPos = cleanMsg.find(':');
    
    std::string typeStr;
    if (separatorPos != std::string::npos) {
        typeStr = cleanMsg.substr(0, separatorPos);
        msg.payload = cleanMsg.substr(separatorPos + 1);
    } else {
        typeStr = cleanMsg;
    }
    
    msg.type = stringToMessageType(typeStr);
    return msg;
}

std::string NetworkProtocol::createConnectMessage(const std::string& playerName) {
    return serialize(MessageType::CONNECT, playerName);
}

std::string NetworkProtocol::createAcceptMessage(char playerRole) {
    return serialize(MessageType::ACCEPT, std::string(1, playerRole));
}

std::string NetworkProtocol::createRejectMessage(const std::string& reason) {
    return serialize(MessageType::REJECT, reason);
}

std::string NetworkProtocol::createMoveMessage(int column) {
    return serialize(MessageType::MOVE, std::to_string(column));
}

std::string NetworkProtocol::createGameStateMessage(const std::string& boardState, char currentPlayer, const std::string& gameStatus) {
    std::ostringstream oss;
    oss << boardState << FIELD_SEPARATOR << currentPlayer << FIELD_SEPARATOR << gameStatus;
    return serialize(MessageType::GAME_STATE, oss.str());
}

std::string NetworkProtocol::createTurnMessage(char player) {
    return serialize(MessageType::TURN, std::string(1, player));
}

std::string NetworkProtocol::createWinMessage(char winner) {
    return serialize(MessageType::WIN, std::string(1, winner));
}

std::string NetworkProtocol::createDrawMessage() {
    return serialize(MessageType::DRAW);
}

std::string NetworkProtocol::createResetMessage() {
    return serialize(MessageType::RESET);
}

std::string NetworkProtocol::createDisconnectMessage() {
    return serialize(MessageType::DISCONNECT);
}

std::string NetworkProtocol::createPingMessage() {
    return serialize(MessageType::PING);
}

std::string NetworkProtocol::createPongMessage() {
    return serialize(MessageType::PONG);
}

std::string NetworkProtocol::parsePlayerName(const NetworkMessage& msg) {
    return msg.payload;
}

char NetworkProtocol::parsePlayerRole(const NetworkMessage& msg) {
    return msg.payload.empty() ? ' ' : msg.payload[0];
}

std::string NetworkProtocol::parseRejectReason(const NetworkMessage& msg) {
    return msg.payload;
}

int NetworkProtocol::parseMoveColumn(const NetworkMessage& msg) {
    try {
        return std::stoi(msg.payload);
    } catch (const std::invalid_argument&) {
        return -1;
    } catch (const std::out_of_range&) {
        return -1;
    }
}

bool NetworkProtocol::parseGameState(const NetworkMessage& msg, std::string& boardState, char& currentPlayer, std::string& gameStatus) {
    std::istringstream iss(msg.payload);
    std::string part;
    std::vector<std::string> parts;
    
    while (std::getline(iss, part, FIELD_SEPARATOR)) {
        parts.push_back(part);
    }
    
    if (parts.size() != 3) {
        return false;
    }
    
    boardState = parts[0];
    currentPlayer = parts[1].empty() ? ' ' : parts[1][0];
    gameStatus = parts[2];
    
    return true;
}

char NetworkProtocol::parseTurnPlayer(const NetworkMessage& msg) {
    return msg.payload.empty() ? ' ' : msg.payload[0];
}

char NetworkProtocol::parseWinner(const NetworkMessage& msg) {
    return msg.payload.empty() ? ' ' : msg.payload[0];
}

std::string NetworkProtocol::messageTypeToString(MessageType type) {
    switch (type) {
        case MessageType::CONNECT: return "CONNECT";
        case MessageType::ACCEPT: return "ACCEPT";
        case MessageType::REJECT: return "REJECT";
        case MessageType::MOVE: return "MOVE";
        case MessageType::GAME_STATE: return "GAME_STATE";
        case MessageType::TURN: return "TURN";
        case MessageType::WIN: return "WIN";
        case MessageType::DRAW: return "DRAW";
        case MessageType::RESET: return "RESET";
        case MessageType::DISCONNECT: return "DISCONNECT";
        case MessageType::PING: return "PING";
        case MessageType::PONG: return "PONG";
        default: return "INVALID";
    }
}

MessageType NetworkProtocol::stringToMessageType(const std::string& typeStr) {
    if (typeStr == "CONNECT") return MessageType::CONNECT;
    if (typeStr == "ACCEPT") return MessageType::ACCEPT;
    if (typeStr == "REJECT") return MessageType::REJECT;
    if (typeStr == "MOVE") return MessageType::MOVE;
    if (typeStr == "GAME_STATE") return MessageType::GAME_STATE;
    if (typeStr == "TURN") return MessageType::TURN;
    if (typeStr == "WIN") return MessageType::WIN;
    if (typeStr == "DRAW") return MessageType::DRAW;
    if (typeStr == "RESET") return MessageType::RESET;
    if (typeStr == "DISCONNECT") return MessageType::DISCONNECT;
    if (typeStr == "PING") return MessageType::PING;
    if (typeStr == "PONG") return MessageType::PONG;
    return MessageType::INVALID;
}

bool NetworkProtocol::isValidMessage(const std::string& message) {
    if (message.empty() || message.back() != MESSAGE_DELIMITER) {
        return false;
    }
    
    NetworkMessage msg = deserialize(message);
    return msg.type != MessageType::INVALID;
}

std::vector<std::string> NetworkProtocol::extractMessages(std::string& buffer) {
    std::vector<std::string> messages;
    
    size_t pos;
    while ((pos = buffer.find(MESSAGE_DELIMITER)) != std::string::npos) {
        std::string message = buffer.substr(0, pos + 1);
        if (isValidMessage(message)) {
            messages.push_back(message);
        }
        buffer.erase(0, pos + 1);
    }
    
    return messages;
}
