---
title: "LAN Multiplayer - Game State Synchronization"
labels: ["enhancement", "multiplayer", "network", "game-logic"]
---

## Description
Implement game state synchronization between server and clients to ensure all players see the same game state at all times. The server is the authoritative source of truth.

## Acceptance Criteria
- [ ] Modify `Game` class to support network synchronization
  - [ ] Add method to serialize game state
  - [ ] Add method to deserialize and apply game state
  - [ ] Add method to validate incoming moves from network
- [ ] Implement server-side move validation
  - [ ] Validate move legality (column not full, valid column number)
  - [ ] Verify it's the correct player's turn
  - [ ] Apply valid moves to authoritative game state
  - [ ] Reject invalid moves with appropriate error messages
- [ ] Implement client-side state updates
  - [ ] Receive and apply full game state updates from server
  - [ ] Update local game display immediately
  - [ ] Disable local moves when it's not the player's turn
- [ ] Add turn management
  - [ ] Server notifies current player when it's their turn
  - [ ] Lock opponent's moves during opponent's turn
  - [ ] Handle turn changes on successful moves
- [ ] Implement win/draw detection synchronization
  - [ ] Server detects win/draw conditions
  - [ ] Broadcast game-over state to all clients
  - [ ] Display appropriate end-game message on all clients

## Technical Details
### Files to Modify
- `include/Game.h` - Add network synchronization methods
- `src/Game.cpp` - Implement synchronization logic
- `include/GameServer.h` - Add game state broadcasting
- `src/GameServer.cpp` - Implement server-side validation
- `include/GameClient.h` - Add state update handling
- `src/GameClient.cpp` - Implement client-side updates

### Game State Serialization Format
Should include:
- Current board state (all cell values)
- Current player turn
- Game status (playing, won, draw)
- Winner (if applicable)
- Move history (optional, for replay)

### Synchronization Strategy
1. **Server Authority**: Server always validates and applies moves first
2. **State Broadcasting**: After each valid move, server broadcasts full state to all clients
3. **Optimistic Client Updates**: Clients can optimistically show their own moves, then sync with server
4. **Conflict Resolution**: Server state always wins in case of conflicts

## Implementation Notes
- Use the existing `Board` class methods for game logic
- Don't duplicate game logic on client - always defer to server
- Implement delta compression (optional optimization for v2)
- Add sequence numbers to detect out-of-order messages
- Consider adding move confirmation messages (ACK/NACK)

## Edge Cases to Handle
- Player makes move when it's not their turn → Server rejects
- Network lag causes delayed move arrival → Server applies in order received
- Client sends invalid column → Server rejects with error
- Game ends while move is in transit → Server ignores late move
- Client and server state drift → Periodic full state sync

## Related Tasks
- Depends on: Task 02 (Client-Server Architecture), Task 03 (Protocol Design)
- Blocks: Task 05 (UI Integration)

## Estimated Effort
8-10 hours
