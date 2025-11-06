---
title: "LAN Multiplayer - UI Integration"
labels: ["enhancement", "multiplayer", "ui", "sdl2"]
---

## Description
Integrate LAN multiplayer functionality into the existing SDL2-based game UI. Add menu screens for hosting/joining games and update the game display to show network status.

## Acceptance Criteria
- [ ] Add multiplayer mode selection to main menu
  - [ ] "Host Game" button
  - [ ] "Join Game" button
  - [ ] Keep existing "Player vs Player" and "Player vs AI" options
- [ ] Create "Host Game" screen
  - [ ] Display server IP address and port
  - [ ] Show "Waiting for opponent..." message
  - [ ] Display connection status
  - [ ] Add "Cancel" button to return to menu
  - [ ] Auto-start game when second player connects
- [ ] Create "Join Game" screen
  - [ ] Input field for server IP address
  - [ ] Input field for port (with default)
  - [ ] "Connect" button
  - [ ] "Back" button to return to menu
  - [ ] Show connection status (connecting, connected, failed)
  - [ ] Display error messages for connection failures
- [ ] Update in-game UI for network play
  - [ ] Show network status indicator (connected, lag, disconnected)
  - [ ] Display opponent's name/identifier
  - [ ] Show "Your turn" / "Opponent's turn" indicator
  - [ ] Disable column selection when it's opponent's turn
  - [ ] Show waiting indicator during opponent's turn
- [ ] Handle network disconnection during game
  - [ ] Display disconnection message
  - [ ] Offer options: "Wait for reconnection" or "Return to menu"
  - [ ] Pause game state during disconnection

## Technical Details
### Files to Modify
- `include/GameUI.h` - Add UI states for network screens
- `src/GameUI.cpp` - Implement network UI screens
- `include/Game.h` - Add network game mode enum value

### New UI States
```cpp
enum class UIState {
    MAIN_MENU,
    MODE_SELECTION,
    HOST_GAME,          // New
    JOIN_GAME,          // New
    WAITING_FOR_OPPONENT, // New
    PLAYING,
    GAME_OVER
};
```

### UI Components Needed
- Text input box (for IP address entry)
- Connection status display
- Network status icon/indicator
- Waiting/loading animation
- Error message modal/popup

### Integration with Network Classes
- `GameUI` should hold references to `GameServer` or `GameClient`
- Use callbacks or polling to update UI based on network events
- Run network operations on separate thread to avoid UI freezing
- Use mutex/locks for thread-safe state access

## Implementation Notes
- Keep network operations non-blocking to maintain UI responsiveness
- Show visual feedback for all network operations
- Implement connection timeout (e.g., 30 seconds)
- Add IP address validation before attempting connection
- Display helpful error messages (e.g., "Connection refused", "Timeout")
- Consider adding a text input component if not already available

## User Experience Flow
### Hosting a Game
1. Main Menu → "Host Game"
2. Display server address (auto-detected or manual)
3. Wait for opponent to connect
4. Show "Opponent connected!" message
5. Auto-start game after 3-second countdown

### Joining a Game
1. Main Menu → "Join Game"
2. Enter server IP address
3. Click "Connect"
4. Show connecting animation
5. On success: transition to game
6. On failure: show error, allow retry

## Related Tasks
- Depends on: Task 02 (Client-Server), Task 04 (Synchronization)
- Works with: Task 06 (Error Handling)

## Estimated Effort
10-14 hours
