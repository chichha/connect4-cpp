---
title: "AI Task 1: Add Game Mode Selection UI"
labels: ["enhancement", "ui", "ai-opponent"]
assignees: []
---

## Description

Add a user interface component that allows players to select between different game modes before starting a game. This includes choosing between Player vs Player (PvP) and Player vs AI modes, and selecting AI difficulty when playing against the computer.

## Acceptance Criteria

- [ ] UI displays a game mode selection screen/menu before starting the game
- [ ] Players can choose between "Player vs Player" and "Player vs AI" modes
- [ ] When "Player vs AI" is selected, UI offers AI difficulty options:
  - [ ] "Easy" (Random AI)
  - [ ] "Medium" (Minimax with depth parameter)
  - [ ] "Hard" (Minimax with higher depth parameter)
- [ ] UI allows selection of minimax search depth (e.g., slider or dropdown with values 1-8)
- [ ] Selected game mode and settings are stored and passed to the game logic
- [ ] UI provides a "Start Game" button to begin with selected settings
- [ ] UI allows returning to mode selection from an active game
- [ ] Visual feedback clearly indicates current selections
- [ ] UI is consistent with existing SDL2-based interface design

## Rationale

A game mode selection UI is the foundation for the AI opponent feature. It provides:
1. **User Choice**: Players can decide whether to play against another human or the computer
2. **Difficulty Control**: Players can adjust AI difficulty to match their skill level
3. **Flexibility**: The depth parameter allows fine-tuning AI strength
4. **Extensibility**: Future AI strategies can be easily added to the selection menu

This must be implemented first as it determines the game flow and which AI opponent (if any) will be instantiated.

## Technical Considerations

- Extend the `GameUI` class to include mode selection rendering
- Add event handling for mode selection buttons/controls
- Store selected mode and AI configuration in appropriate data structures
- Consider adding a `GameMode` enum (e.g., `PVP`, `PVE_RANDOM`, `PVE_MINIMAX`)
- UI should appear before the main game board or as an overlay
- Depth parameter should have reasonable bounds (suggested: 1-8, default: 4)

## Estimated Effort

6-8 hours

## Phase

Phase 1: AI Foundation

## Dependencies

None - This is the first task in the AI opponent feature

## Related Tasks

- AI Task 2: Implement Random AI Player
- AI Task 3: Implement Minimax AI with Configurable Depth

## Testing Checklist

- [ ] Mode selection screen renders correctly
- [ ] All buttons/controls are clickable and responsive
- [ ] Selected options are visually highlighted
- [ ] "Start Game" button launches game with correct configuration
- [ ] Can switch modes multiple times before starting
- [ ] Depth parameter accepts valid range and rejects invalid values
- [ ] UI degrades gracefully if future AI types are not yet implemented
