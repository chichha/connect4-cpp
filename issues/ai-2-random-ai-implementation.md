---
title: "AI Task 2: Implement Random AI Player"
labels: ["enhancement", "ai-opponent", "algorithm"]
assignees: []
---

## Description

Implement a random AI player that selects valid moves randomly. This serves as the "Easy" difficulty option and provides a baseline AI opponent for testing the AI infrastructure.

## Acceptance Criteria

- [ ] Create an `AIPlayer` class or interface that defines AI player behavior
- [ ] Implement `RandomAI` class that extends/implements the AI player interface
- [ ] AI can analyze the current board state to identify valid moves
- [ ] AI randomly selects from all valid (non-full) columns
- [ ] AI selection is performed efficiently (O(n) where n = number of columns)
- [ ] Random selection uses proper random number generation (not predictable patterns)
- [ ] AI returns the selected column number to the game logic
- [ ] Integration with `Game` class allows AI to make moves automatically
- [ ] AI respects turn order and game rules
- [ ] AI can be instantiated when "Easy" mode is selected in the UI

## Rationale

A random AI provides:
1. **Simplicity**: Easy to implement and test, serving as the foundation for AI infrastructure
2. **Baseline**: Provides a weak opponent for beginners and testing purposes
3. **Architecture**: Establishes the AI player interface that more sophisticated AIs will implement
4. **Validation**: Proves the game can work with computer-controlled players

The random AI is intentionally simple but must be well-structured to support future AI implementations.

## Technical Considerations

**Class Design:**
```cpp
// Suggested interface
class AIPlayer {
public:
    virtual int selectMove(const Board& board) = 0;
    virtual ~AIPlayer() = default;
};

class RandomAI : public AIPlayer {
public:
    int selectMove(const Board& board) override;
private:
    // Use <random> library for proper randomization
    std::random_device rd;
    std::mt19937 gen;
};
```

**Implementation Details:**
- Check each column using `board.isColumnFull(col)` to build list of valid moves
- Use `std::uniform_int_distribution` for uniform random selection
- Seed random number generator properly (avoid using `rand()`)
- Handle edge case where board is full (though this should be caught by game logic)

**Integration:**
- Modify `Game` class to optionally hold an `AIPlayer*` for Player 2 (or Player 1)
- When AI's turn, call `aiPlayer->selectMove(board)` instead of `getPlayerMove()`
- Ensure AI moves trigger same validation and win-checking as human moves

## Estimated Effort

4-6 hours

## Phase

Phase 2: AI Implementation

## Dependencies

- AI Task 1: Add Game Mode Selection UI (to enable "Easy" mode selection)

## Related Tasks

- AI Task 1: Add Game Mode Selection UI
- AI Task 3: Implement Minimax AI with Configurable Depth

## Testing Checklist

- [ ] RandomAI can be instantiated
- [ ] `selectMove()` returns a valid column (0-6 for standard board)
- [ ] `selectMove()` never returns a full column
- [ ] Multiple calls produce different moves (verify randomness)
- [ ] AI can play a complete game without errors
- [ ] AI respects game rules (pieces drop correctly, turn alternates)
- [ ] Game detects wins/draws correctly when AI is playing
- [ ] Performance is acceptable (move selection takes < 100ms)
- [ ] AI works with different board states (empty, partially full, almost full)

## Implementation Notes

**Files to Create/Modify:**
- `include/AIPlayer.h` - Base class/interface
- `include/RandomAI.h` - Random AI declaration
- `src/RandomAI.cpp` - Random AI implementation
- `include/Game.h` - Add AI player support
- `src/Game.cpp` - Integrate AI into game loop
- `src/GameUI.cpp` - Connect UI to AI-enabled game

**Example Usage:**
```cpp
// In Game class
if (isAITurn() && aiPlayer) {
    int column = aiPlayer->selectMove(board);
    makeMove(column);
}
```
