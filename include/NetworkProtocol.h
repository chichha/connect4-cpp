#ifndef NETWORKPROTOCOL_H
#define NETWORKPROTOCOL_H

#include <string>
#include <vector>

// Message Types
enum class MessageType {
    CONNECT,        // Client connection request with player name
    ACCEPT,         // Server accepts connection, assigns player role
    REJECT,         // Server rejects connection
    MOVE,           // Player move (column number)
    GAME_STATE,     // Full game state update
    TURN,           // Notify player of their turn
    WIN,            // Game over, player wins
    DRAW,           // Game over, draw
    RESET,          // Start new game
    DISCONNECT,     // Player disconnection notification
    PING,           // Keep-alive ping
    PONG,           // Keep-alive pong response
    INVALID         // Invalid message
};

// Message structure
struct NetworkMessage {
    MessageType type;
    std::string payload;
    
    NetworkMessage() : type(MessageType::INVALID) {}
    NetworkMessage(MessageType t, const std::string& p = "") : type(t), payload(p) {}
};

class NetworkProtocol {
public:
    // Message serialization/deserialization
    static std::string serialize(MessageType type, const std::string& payload = "");
    static NetworkMessage deserialize(const std::string& message);
    
    // Helper methods to create specific messages
    static std::string createConnectMessage(const std::string& playerName);
    static std::string createAcceptMessage(char playerRole);
    static std::string createRejectMessage(const std::string& reason);
    static std::string createMoveMessage(int column);
    static std::string createGameStateMessage(const std::string& boardState, char currentPlayer, const std::string& gameStatus);
    static std::string createTurnMessage(char player);
    static std::string createWinMessage(char winner);
    static std::string createDrawMessage();
    static std::string createResetMessage();
    static std::string createDisconnectMessage();
    static std::string createPingMessage();
    static std::string createPongMessage();
    
    // Helper methods to parse message payloads
    static std::string parsePlayerName(const NetworkMessage& msg);
    static char parsePlayerRole(const NetworkMessage& msg);
    static std::string parseRejectReason(const NetworkMessage& msg);
    static int parseMoveColumn(const NetworkMessage& msg);
    static bool parseGameState(const NetworkMessage& msg, std::string& boardState, char& currentPlayer, std::string& gameStatus);
    static char parseTurnPlayer(const NetworkMessage& msg);
    static char parseWinner(const NetworkMessage& msg);
    
    // Message type to string conversion
    static std::string messageTypeToString(MessageType type);
    static MessageType stringToMessageType(const std::string& typeStr);
    
    // Validation
    static bool isValidMessage(const std::string& message);
    
    // Message buffering utilities
    static std::vector<std::string> extractMessages(std::string& buffer);
    
private:
    static const char MESSAGE_DELIMITER = '\n';
    static const char FIELD_SEPARATOR = '|';
};

#endif // NETWORKPROTOCOL_H
