---
title: "LAN Multiplayer - Client-Server Architecture"
labels: ["enhancement", "multiplayer", "network", "architecture"]
---

## Description
Implement the client-server architecture for LAN multiplayer games. The server will host the game and manage game state, while clients connect to play.

## Acceptance Criteria
- [ ] Create a `GameServer` class to manage hosted games
  - [ ] Accept incoming client connections
  - [ ] Maintain game state as the authoritative source
  - [ ] Handle multiple game sessions (optional for v1)
  - [ ] Broadcast game updates to connected clients
- [ ] Create a `GameClient` class to connect to servers
  - [ ] Connect to server by IP address and port
  - [ ] Send player moves to server
  - [ ] Receive and process game state updates
  - [ ] Handle disconnections gracefully
- [ ] Implement connection handshake protocol
  - [ ] Player identification and role assignment (Player 1 vs Player 2)
  - [ ] Game configuration synchronization
- [ ] Add connection status monitoring (connected, disconnected, error states)

## Technical Details
### Files to Create
- `include/GameServer.h` - Server class declaration
- `include/GameClient.h` - Client class declaration
- `src/GameServer.cpp` - Server implementation
- `src/GameClient.cpp` - Client implementation

### Server Responsibilities
- Host the game on a specified port
- Accept exactly 2 player connections (for Connect 4)
- Validate all moves received from clients
- Maintain authoritative game state
- Broadcast state changes to all clients
- Detect and handle client disconnections

### Client Responsibilities
- Connect to server via IP:Port
- Send local player moves to server
- Receive and apply game state updates
- Handle network errors and disconnections
- Notify UI of connection status changes

## Implementation Notes
- Server should use the existing `Game` class for game logic
- Client should synchronize with server's game state (not maintain independent logic)
- Use threading or async I/O to avoid blocking the game UI
- Consider using a separate thread for network operations
- Implement a message queue for incoming/outgoing messages

## Related Tasks
- Depends on: Task 01 (Network Infrastructure)
- Blocks: Task 04 (Game Synchronization)

## Estimated Effort
8-12 hours
