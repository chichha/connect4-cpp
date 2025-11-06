#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <string>
#include <cstdint>

// Platform-specific includes
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    typedef SOCKET SocketHandle;
    #define INVALID_SOCKET_VALUE INVALID_SOCKET
    #define SOCKET_ERROR_VALUE SOCKET_ERROR
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    typedef int SocketHandle;
    #define INVALID_SOCKET_VALUE -1
    #define SOCKET_ERROR_VALUE -1
#endif

class NetworkManager {
public:
    static const int DEFAULT_PORT = 4444;
    static const int BUFFER_SIZE = 4096;
    static const int DEFAULT_TIMEOUT_SECONDS = 30;

    NetworkManager();
    ~NetworkManager();

    // Socket lifecycle
    bool initialize();
    void cleanup();
    
    // Server operations
    bool createServerSocket(int port);
    bool listen(int backlog = 5);
    SocketHandle acceptConnection();
    
    // Client operations
    bool connectToServer(const std::string& ipAddress, int port, int timeoutSeconds = DEFAULT_TIMEOUT_SECONDS);
    
    // Data transmission
    int send(const char* data, int length);
    int receive(char* buffer, int bufferSize);
    
    // Socket utilities
    void closeSocket();
    bool setNonBlocking(bool nonBlocking);
    bool isConnected() const;
    std::string getLastError() const;
    
    // Get local IP address
    static std::string getLocalIPAddress();

private:
    SocketHandle socket_;
    bool initialized_;
    bool connected_;
    std::string lastError_;
    
    void setLastError(const std::string& error);
    int getLastSocketError() const;
};

#endif // NETWORKMANAGER_H
