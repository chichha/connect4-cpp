#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include "NetworkManager.h"
#include "NetworkProtocol.h"
#include "Game.h"
#include <string>
#include <thread>
#include <mutex>
#include <atomic>

enum class ClientState {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    IN_GAME,
    ERROR
};

class GameClient {
public:
    GameClient();
    ~GameClient();
    
    // Client lifecycle
    bool connect(const std::string& serverIP, int port = NetworkManager::DEFAULT_PORT);
    void disconnect();
    
    // Client state
    ClientState getState() const;
    bool isConnected() const;
    char getPlayerRole() const;
    
    // Game control
    bool sendMove(int column);
    bool requestReset();
    const Game& getGame() const;
    
    // Status
    std::string getStatusMessage() const;
    std::string getLastError() const;
    
private:
    // Network
    NetworkManager socket_;
    std::string receiveBuffer_;
    
    // Game state
    Game game_;
    char playerRole_;
    
    // Client state
    ClientState state_;
    std::atomic<bool> running_;
    std::string statusMessage_;
    std::string lastError_;
    
    // Threading
    std::thread receiveThread_;
    std::mutex stateMutex_;
    
    // Thread functions
    void receiveLoopThread();
    
    // Message processing
    void processMessage(const NetworkMessage& msg);
    
    // Helper methods
    void setStatus(const std::string& message);
    void setError(const std::string& error);
};

#endif // GAMECLIENT_H
