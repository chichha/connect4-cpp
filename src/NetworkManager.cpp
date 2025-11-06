#include "NetworkManager.h"
#include <cstring>
#include <sstream>

#ifdef _WIN32
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <ifaddrs.h>
    #include <net/if.h>
#endif

NetworkManager::NetworkManager() 
    : socket_(INVALID_SOCKET_VALUE), initialized_(false), connected_(false) {
}

NetworkManager::~NetworkManager() {
    cleanup();
}

bool NetworkManager::initialize() {
#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        setLastError("WSAStartup failed with error: " + std::to_string(result));
        return false;
    }
#endif
    initialized_ = true;
    return true;
}

void NetworkManager::cleanup() {
    closeSocket();
#ifdef _WIN32
    if (initialized_) {
        WSACleanup();
    }
#endif
    initialized_ = false;
}

bool NetworkManager::createServerSocket(int port) {
    if (!initialized_) {
        setLastError("NetworkManager not initialized");
        return false;
    }

    // Create socket
    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_ == INVALID_SOCKET_VALUE) {
        setLastError("Failed to create socket");
        return false;
    }

    // Set socket options to reuse address
    int optval = 1;
#ifdef _WIN32
    setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
#else
    setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
#endif

    // Bind socket to port
    struct sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(socket_, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR_VALUE) {
        setLastError("Failed to bind socket to port " + std::to_string(port));
        closeSocket();
        return false;
    }

    return true;
}

bool NetworkManager::listen(int backlog) {
    if (socket_ == INVALID_SOCKET_VALUE) {
        setLastError("No server socket created");
        return false;
    }

    if (::listen(socket_, backlog) == SOCKET_ERROR_VALUE) {
        setLastError("Failed to listen on socket");
        return false;
    }

    return true;
}

SocketHandle NetworkManager::acceptConnection() {
    if (socket_ == INVALID_SOCKET_VALUE) {
        setLastError("No server socket created");
        return INVALID_SOCKET_VALUE;
    }

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    
    SocketHandle clientSocket = accept(socket_, (struct sockaddr*)&clientAddr, &clientAddrLen);
    
    if (clientSocket == INVALID_SOCKET_VALUE) {
        setLastError("Failed to accept connection");
        return INVALID_SOCKET_VALUE;
    }

    return clientSocket;
}

bool NetworkManager::connectToServer(const std::string& ipAddress, int port, int timeoutSeconds) {
    if (!initialized_) {
        setLastError("NetworkManager not initialized");
        return false;
    }

    // Create socket
    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_ == INVALID_SOCKET_VALUE) {
        setLastError("Failed to create socket");
        return false;
    }

    // Set socket to non-blocking for timeout support
    if (timeoutSeconds > 0) {
        setNonBlocking(true);
    }

    // Set up server address
    struct sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    
#ifdef _WIN32
    serverAddr.sin_addr.s_addr = inet_addr(ipAddress.c_str());
#else
    if (inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr) <= 0) {
        setLastError("Invalid IP address format");
        closeSocket();
        return false;
    }
#endif

    // Attempt to connect
    int result = connect(socket_, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    
    if (result == SOCKET_ERROR_VALUE) {
#ifdef _WIN32
        int error = WSAGetLastError();
        if (error != WSAEWOULDBLOCK) {
#else
        if (errno != EINPROGRESS) {
#endif
            setLastError("Connection failed");
            closeSocket();
            return false;
        }

        // Wait for connection with timeout
        fd_set writeSet;
        FD_ZERO(&writeSet);
        FD_SET(socket_, &writeSet);
        
        struct timeval timeout;
        timeout.tv_sec = timeoutSeconds;
        timeout.tv_usec = 0;
        
        result = select(socket_ + 1, nullptr, &writeSet, nullptr, &timeout);
        
        if (result <= 0) {
            setLastError("Connection timeout");
            closeSocket();
            return false;
        }

        // Check if connection was successful
        int optval;
        socklen_t optlen = sizeof(optval);
        getsockopt(socket_, SOL_SOCKET, SO_ERROR, (char*)&optval, &optlen);
        
        if (optval != 0) {
            setLastError("Connection failed");
            closeSocket();
            return false;
        }
    }

    // Set back to blocking mode
    if (timeoutSeconds > 0) {
        setNonBlocking(false);
    }

    connected_ = true;
    return true;
}

int NetworkManager::send(const char* data, int length) {
    if (socket_ == INVALID_SOCKET_VALUE) {
        setLastError("No socket available");
        return -1;
    }

    int totalSent = 0;
    while (totalSent < length) {
        int sent = ::send(socket_, data + totalSent, length - totalSent, 0);
        if (sent == SOCKET_ERROR_VALUE) {
            setLastError("Send failed");
            connected_ = false;
            return -1;
        }
        totalSent += sent;
    }

    return totalSent;
}

int NetworkManager::receive(char* buffer, int bufferSize) {
    if (socket_ == INVALID_SOCKET_VALUE) {
        setLastError("No socket available");
        return -1;
    }

    int received = recv(socket_, buffer, bufferSize, 0);
    
    if (received == SOCKET_ERROR_VALUE) {
        setLastError("Receive failed");
        connected_ = false;
        return -1;
    } else if (received == 0) {
        // Connection closed gracefully
        connected_ = false;
        return 0;
    }

    return received;
}

void NetworkManager::closeSocket() {
    if (socket_ != INVALID_SOCKET_VALUE) {
#ifdef _WIN32
        closesocket(socket_);
#else
        ::close(socket_);
#endif
        socket_ = INVALID_SOCKET_VALUE;
        connected_ = false;
    }
}

bool NetworkManager::setNonBlocking(bool nonBlocking) {
    if (socket_ == INVALID_SOCKET_VALUE) {
        return false;
    }

#ifdef _WIN32
    u_long mode = nonBlocking ? 1 : 0;
    return ioctlsocket(socket_, FIONBIO, &mode) == 0;
#else
    int flags = fcntl(socket_, F_GETFL, 0);
    if (flags == -1) return false;
    
    flags = nonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
    return fcntl(socket_, F_SETFL, flags) == 0;
#endif
}

bool NetworkManager::isConnected() const {
    return connected_;
}

std::string NetworkManager::getLastError() const {
    return lastError_;
}

void NetworkManager::setLastError(const std::string& error) {
    int socketError = getLastSocketError();
    std::ostringstream oss;
    oss << error;
    if (socketError != 0) {
        oss << " (Error code: " << socketError << ")";
    }
    lastError_ = oss.str();
}

int NetworkManager::getLastSocketError() const {
#ifdef _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

std::string NetworkManager::getLocalIPAddress() {
    std::string ipAddress = "127.0.0.1"; // Default to localhost

#ifdef _WIN32
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        struct hostent* host = gethostbyname(hostname);
        if (host != nullptr && host->h_addr_list[0] != nullptr) {
            struct in_addr addr;
            std::memcpy(&addr, host->h_addr_list[0], sizeof(struct in_addr));
            ipAddress = inet_ntoa(addr);
        }
    }
#else
    struct ifaddrs* ifAddrStruct = nullptr;
    if (getifaddrs(&ifAddrStruct) == 0) {
        for (struct ifaddrs* ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr == nullptr) continue;
            
            if (ifa->ifa_addr->sa_family == AF_INET) {
                // Skip loopback
                if (ifa->ifa_flags & IFF_LOOPBACK) continue;
                
                void* tmpAddrPtr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
                char addressBuffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
                ipAddress = addressBuffer;
                break; // Use first non-loopback IPv4 address
            }
        }
        if (ifAddrStruct != nullptr) {
            freeifaddrs(ifAddrStruct);
        }
    }
#endif

    return ipAddress;
}
