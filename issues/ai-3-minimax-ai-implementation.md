---
title: "AI Task 3: Implement Minimax AI with Configurable Depth"
labels: ["enhancement", "ai-opponent", "algorithm", "advanced"]
assignees: []
---

## Description

Implement a minimax AI player with alpha-beta pruning and configurable search depth. This provides "Medium" and "Hard" difficulty options, creating a challenging opponent that uses game tree search to find optimal moves.

## Acceptance Criteria

- [ ] Implement `MinimaxAI` class that extends the AI player interface
- [ ] Algorithm uses minimax with alpha-beta pruning for efficiency
- [ ] Search depth is configurable via constructor parameter
- [ ] AI evaluates board positions with a heuristic scoring function
- [ ] AI considers winning positions as highest priority
- [ ] AI blocks opponent's winning moves
- [ ] AI creates tactical opportunities (e.g., double threats)
- [ ] Performance is acceptable even at higher depths (depth 6-8)
- [ ] AI returns the best move according to minimax evaluation
- [ ] Algorithm handles both maximizing (AI) and minimizing (opponent) perspectives
- [ ] Depth parameter can be set from UI (range: 1-8 recommended)
- [ ] AI can be instantiated when "Medium" or "Hard" mode is selected

## Rationale

Minimax AI provides:
1. **Challenge**: Creates a strong opponent that plays strategically
2. **Configurability**: Depth parameter allows users to adjust difficulty
3. **Educational Value**: Demonstrates classic game AI algorithm
4. **Competitive Play**: Makes single-player mode engaging and replayable

Alpha-beta pruning is essential for reasonable performance at deeper search levels.

## Technical Considerations

**Algorithm Overview:**
```cpp
class MinimaxAI : public AIPlayer {
public:
    MinimaxAI(int depth = 4);
    int selectMove(const Board& board) override;
    
private:
    int depth;
    
    // Core minimax with alpha-beta pruning
    int minimax(Board& board, int currentDepth, int alpha, int beta, bool isMaximizing);
    
    // Board evaluation heuristic
    int evaluateBoard(const Board& board);
    
    // Helper functions
    int scorePosition(const Board& board, int row, int col, char player);
    std::vector<int> getValidMoves(const Board& board);
};
```

**Heuristic Scoring Function:**
- Win for AI: +1,000,000 (or INT_MAX / 2)
- Win for opponent: -1,000,000
- Three-in-a-row with empty space: +100
- Two-in-a-row with empty spaces: +10
- Center column preference: +3 (center control is valuable)
- Consider both horizontal, vertical, and diagonal patterns
- Evaluate entire board state, not just one position

**Alpha-Beta Pruning:**
- Alpha: best value maximizer can guarantee
- Beta: best value minimizer can guarantee
- Prune branches where beta <= alpha

**Performance Optimizations:**
- Move ordering: try center columns first (better pruning)
- Iterative deepening (optional): can help with time constraints
- Transposition table (advanced, optional): cache evaluated positions
- Early termination for forced wins/losses

**Depth Guidelines:**
- Depth 1-2: Very weak, mostly tactical
- Depth 3-4: Decent play, good for "Medium" difficulty
- Depth 5-6: Strong play, suitable for "Hard" difficulty
- Depth 7-8: Very strong, may be slow on some systems

## Estimated Effort

10-15 hours (most complex task in the AI feature)

## Phase

Phase 2: AI Implementation

## Dependencies

- AI Task 1: Add Game Mode Selection UI (to pass depth parameter)
- AI Task 2: Implement Random AI Player (establishes AI infrastructure)

## Related Tasks

- AI Task 1: Add Game Mode Selection UI
- AI Task 2: Implement Random AI Player

## Testing Checklist

### Correctness Testing
- [ ] AI makes legal moves only
- [ ] AI wins when it has a winning move available
- [ ] AI blocks opponent's winning move (depth >= 2)
- [ ] AI creates double-threat opportunities (depth >= 4)
- [ ] AI plays optimally in endgame positions

### Performance Testing
- [ ] Depth 1: < 100ms per move
- [ ] Depth 4: < 2 seconds per move
- [ ] Depth 6: < 10 seconds per move (acceptable for hard mode)
- [ ] No stack overflow or memory issues at max depth

### Depth Configuration Testing
- [ ] Can instantiate with different depths (1-8)
- [ ] Higher depth plays observably better than lower depth
- [ ] Depth 1 should be noticeably weaker than RandomAI patterns
- [ ] UI depth selection is respected by the AI

### Integration Testing
- [ ] Works with game loop and UI
- [ ] Can play complete games without crashes
- [ ] Win/draw detection works correctly
- [ ] Can switch between Random and Minimax AI
- [ ] Performance is acceptable during actual gameplay

### Edge Cases
- [ ] Handles empty board correctly
- [ ] Handles nearly full board correctly
- [ ] Handles positions with immediate win/loss
- [ ] Handles stalemate/draw positions

## Implementation Notes

**Files to Create/Modify:**
- `include/MinimaxAI.h` - Minimax AI declaration
- `src/MinimaxAI.cpp` - Minimax AI implementation
- `src/Game.cpp` - Support different AI types based on mode
- `src/GameUI.cpp` - Pass depth parameter from UI to AI

**Example Evaluation Function:**
```cpp
int MinimaxAI::evaluateBoard(const Board& board) {
    // Check for wins
    if (board.checkWin(AI_PLAYER)) return 1000000;
    if (board.checkWin(HUMAN_PLAYER)) return -1000000;
    
    int score = 0;
    
    // Evaluate all windows (4-cell sequences)
    for (each possible window) {
        score += evaluateWindow(window);
    }
    
    // Center column preference
    score += centerColumnCount * 3;
    
    return score;
}
```

**Testing Strategy:**
1. Start with simple positions where optimal move is clear
2. Test against RandomAI (Minimax should win consistently)
3. Test AI vs AI (same depth should be ~50/50, higher depth should win)
4. Profile performance at different depths
5. Play manual games to verify AI "feels" intelligent

## References

- Minimax Algorithm: https://en.wikipedia.org/wiki/Minimax
- Alpha-Beta Pruning: https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
- Connect 4 is a solved game; perfect play leads to first player win with optimal strategy
- For Connect 4 specific heuristics, research "Connect 4 evaluation function"
