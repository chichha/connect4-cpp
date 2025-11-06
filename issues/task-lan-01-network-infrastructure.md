---
title: "LAN Multiplayer - Network Infrastructure Setup"
labels: ["enhancement", "multiplayer", "network"]
---

## Description
Set up the core network infrastructure for LAN multiplayer using TCP sockets. This task establishes the foundation for client-server communication.

## Acceptance Criteria
- [ ] Create a `NetworkManager` class to handle TCP socket operations
- [ ] Implement cross-platform socket support (Windows Winsock, Linux/macOS POSIX sockets)
- [ ] Add socket initialization and cleanup functions
- [ ] Implement basic connection establishment (connect/accept)
- [ ] Add send/receive methods for raw data transmission
- [ ] Handle socket errors gracefully with appropriate error codes
- [ ] Update CMakeLists.txt to link platform-specific networking libraries (ws2_32 on Windows)
- [ ] Create unit tests for basic socket operations

## Technical Details
### Files to Create
- `include/NetworkManager.h` - Network manager class declaration
- `src/NetworkManager.cpp` - Network manager implementation
- `tests/NetworkManagerTest.cpp` (optional) - Unit tests

### Platform Considerations
- **Windows**: Use Winsock2 (`ws2_32.lib`)
- **Linux/macOS**: Use POSIX sockets

### Dependencies
- Standard library networking headers (`<sys/socket.h>`, `<netinet/in.h>`, `<arpa/inet.h>` for Unix)
- Windows: `<winsock2.h>`, `<ws2tcpip.h>`

## Implementation Notes
- Use RAII pattern for socket management (close sockets in destructor)
- Support both blocking and non-blocking socket modes
- Implement timeout mechanisms for connection attempts
- Consider using a constant for default port (e.g., 4444)

## Related Tasks
- Blocked by: None
- Blocks: Task 02 (Client-Server Architecture), Task 03 (Protocol Design)

## Estimated Effort
4-6 hours
