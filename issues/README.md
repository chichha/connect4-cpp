# GitHub Issues - AI Opponent Feature

This directory contains structured task definitions for adding an AI opponent feature to the Connect 4 game, ready to be converted into GitHub issues.

## Overview

There are **3 tasks** for implementing AI opponents with varying difficulty levels.

## Feature: Add AI Opponent

The AI opponent feature allows players to play against the computer instead of requiring two human players. It includes:

1. **Game Mode Selection UI** - Choose between Player vs Player or Player vs AI
2. **Random AI (Easy)** - A simple AI that makes random valid moves
3. **Minimax AI (Medium/Hard)** - An intelligent AI using game tree search with configurable difficulty

## Quick Start

### Option 1: GitHub CLI (Recommended)

Create individual issues using the GitHub CLI:

```bash
# Install and authenticate GitHub CLI first
gh auth login

# Create all AI opponent issues
gh issue create \
  --repo chichha/connect4-cpp \
  --title "AI Task 1: Add Game Mode Selection UI" \
  --body-file issues/ai-1-game-mode-selection-ui.md \
  --label "enhancement,ui,ai-opponent"

gh issue create \
  --repo chichha/connect4-cpp \
  --title "AI Task 2: Implement Random AI Player" \
  --body-file issues/ai-2-random-ai-implementation.md \
  --label "enhancement,ai-opponent,algorithm"

gh issue create \
  --repo chichha/connect4-cpp \
  --title "AI Task 3: Implement Minimax AI with Configurable Depth" \
  --body-file issues/ai-3-minimax-ai-implementation.md \
  --label "enhancement,ai-opponent,algorithm,advanced"
```

### Option 2: Manual Creation

You can manually create issues by copying the content from each markdown file:

1. Go to https://github.com/chichha/connect4-cpp/issues/new
2. Open a task file (e.g., `ai-1-game-mode-selection-ui.md`)
3. Copy the title from the frontmatter
4. Copy the body content (everything after the second `---`)
5. Add the labels specified in the frontmatter
6. Create the issue

### Option 3: Automated Script

You can create a shell script to automate issue creation:

```bash
#!/bin/bash

# Create AI Task 1
gh issue create \
  --repo chichha/connect4-cpp \
  --title "AI Task 1: Add Game Mode Selection UI" \
  --body-file issues/ai-1-game-mode-selection-ui.md \
  --label "enhancement,ui,ai-opponent"

sleep 1

# Create AI Task 2
gh issue create \
  --repo chichha/connect4-cpp \
  --title "AI Task 2: Implement Random AI Player" \
  --body-file issues/ai-2-random-ai-implementation.md \
  --label "enhancement,ai-opponent,algorithm"

sleep 1

# Create AI Task 3
gh issue create \
  --repo chichha/connect4-cpp \
  --title "AI Task 3: Implement Minimax AI with Configurable Depth" \
  --body-file issues/ai-3-minimax-ai-implementation.md \
  --label "enhancement,ai-opponent,algorithm,advanced"

echo "All AI opponent issues created successfully!"
```

## Task List

### Phase 1: AI Foundation (1 task)
- **ai-1-game-mode-selection-ui.md** - Add UI to select game mode (PvP or PvE) and AI difficulty
  - Labels: `enhancement`, `ui`, `ai-opponent`
  - Estimated Effort: 6-8 hours
  - Dependencies: None

### Phase 2: AI Implementation (2 tasks)
- **ai-2-random-ai-implementation.md** - Implement random AI for "Easy" difficulty
  - Labels: `enhancement`, `ai-opponent`, `algorithm`
  - Estimated Effort: 4-6 hours
  - Dependencies: AI Task 1

- **ai-3-minimax-ai-implementation.md** - Implement minimax AI with configurable depth for "Medium/Hard" difficulty
  - Labels: `enhancement`, `ai-opponent`, `algorithm`, `advanced`
  - Estimated Effort: 10-15 hours
  - Dependencies: AI Task 1, AI Task 2

## Implementation Order

The tasks should be implemented in the following order:

1. **AI Task 1** (Game Mode Selection UI)
   - Provides the foundation for selecting AI opponents
   - Establishes UI patterns for AI configuration
   - Must be completed before AI players are useful

2. **AI Task 2** (Random AI)
   - Establishes the AI player architecture
   - Provides immediate value with "Easy" mode
   - Tests the game's ability to work with computer players
   - Simpler than minimax, good for validating the infrastructure

3. **AI Task 3** (Minimax AI)
   - Builds on the AI architecture from Task 2
   - Provides challenging "Medium" and "Hard" difficulty levels
   - Most complex task, benefits from having infrastructure already in place

## Total Estimated Effort

**20-29 hours** across all three tasks

## Labels Used

- `enhancement` - Feature enhancements
- `ui` - User interface changes
- `ai-opponent` - AI opponent feature
- `algorithm` - Algorithm implementation
- `advanced` - Advanced/complex tasks

## File Format

Each task file follows this structure:

```markdown
---
title: "Task Title"
labels: ["label1", "label2"]
assignees: []
---

## Description
...

## Acceptance Criteria
- [ ] Criterion 1
- [ ] Criterion 2

## Rationale
...

## Technical Considerations
...

## Estimated Effort
X-Y hours

## Phase
...

## Dependencies
...

## Related Tasks
...

## Testing Checklist
...
```

## Prerequisites

To use the GitHub CLI, you need:

1. **GitHub CLI** - Install from https://cli.github.com/
2. **Authentication** - Run `gh auth login` to authenticate
3. **Repository Access** - Write access to the repository

## Architecture Overview

### AI Player Interface
```cpp
class AIPlayer {
public:
    virtual int selectMove(const Board& board) = 0;
    virtual ~AIPlayer() = default;
};
```

### Random AI (Easy)
- Randomly selects from valid columns
- No strategic thinking
- Fast execution (< 100ms)

### Minimax AI (Medium/Hard)
- Uses minimax algorithm with alpha-beta pruning
- Configurable search depth (1-8)
- Strategic play with tactical awareness
- Execution time varies by depth

### Game Integration
- Game class supports both human and AI players
- UI allows selecting player types before game starts
- AI moves are triggered automatically on AI's turn

## Testing Strategy

Each task includes a comprehensive testing checklist covering:
- Correctness (legal moves, proper behavior)
- Performance (acceptable response times)
- Integration (works with existing game logic)
- Edge cases (empty board, full board, etc.)
- User experience (smooth gameplay, clear feedback)

## Notes

- The AI feature is designed to be modular and extensible
- Additional AI strategies can be added in the future by implementing the `AIPlayer` interface
- The minimax AI can be tuned by adjusting the evaluation function
- Consider adding move time limits for higher depths to prevent UI freezing

## Troubleshooting

**"gh command not found"**
- Install GitHub CLI from https://cli.github.com/

**"Authentication failed"**
- Run `gh auth login` and follow the prompts

**"Permission denied"**
- Ensure you have write access to the repository

**Issues not created**
- Check that you're in the repository root directory
- Verify the issue file paths are correct
- Check GitHub API rate limits

## See Also

- [README.md](../README.md) - Main project documentation
- [Connect 4 Strategy](https://en.wikipedia.org/wiki/Connect_Four) - Game strategy and analysis
- [Minimax Algorithm](https://en.wikipedia.org/wiki/Minimax) - Algorithm reference
- [Alpha-Beta Pruning](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning) - Optimization technique
