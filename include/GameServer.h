#ifndef GAMESERVER_H
#define GAMESERVER_H

#include "NetworkManager.h"
#include "NetworkProtocol.h"
#include "Game.h"
#include <string>
#include <thread>
#include <mutex>
#include <atomic>

enum class ServerState {
    IDLE,
    WAITING_FOR_PLAYERS,
    GAME_IN_PROGRESS,
    ERROR
};

class GameServer {
public:
    GameServer();
    ~GameServer();
    
    // Server lifecycle
    bool start(int port = NetworkManager::DEFAULT_PORT);
    void stop();
    
    // Server state
    ServerState getState() const;
    std::string getIPAddress() const;
    int getPort() const;
    int getConnectedPlayerCount() const;
    bool isGameStarted() const;
    
    // Game control
    bool processPlayerMove(int column, char player);
    const Game& getGame() const;
    
    // Get status messages
    std::string getStatusMessage() const;
    std::string getLastError() const;
    
private:
    // Network
    NetworkManager serverSocket_;
    SocketHandle client1Socket_;
    SocketHandle client2Socket_;
    std::string receiveBuffer1_;
    std::string receiveBuffer2_;
    
    // Game state
    Game game_;
    char player1Char_;
    char player2Char_;
    
    // Server state
    ServerState state_;
    int port_;
    std::atomic<bool> running_;
    std::atomic<int> connectedPlayers_;
    std::string statusMessage_;
    std::string lastError_;
    
    // Threading
    std::thread acceptThread_;
    std::thread gameThread_;
    std::mutex stateMutex_;
    
    // Thread functions
    void acceptConnectionsThread();
    void gameLoopThread();
    
    // Network operations
    void handleClient(SocketHandle clientSocket, int playerNum);
    void processMessage(const NetworkMessage& msg, int playerNum);
    void broadcastGameState();
    void sendToClient(SocketHandle socket, const std::string& message);
    
    // Helper methods
    void setStatus(const std::string& message);
    void setError(const std::string& error);
    bool sendData(SocketHandle socket, const char* data, int length);
    int receiveData(SocketHandle socket, char* buffer, int bufferSize);
};

#endif // GAMESERVER_H
