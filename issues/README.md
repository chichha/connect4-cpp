# GitHub Issues - UI Migration Tasks

This directory contains structured task definitions for the UI migration feature, ready to be converted into GitHub issues.

## Overview

There are **20 tasks** organized across **7 phases** for migrating the Connect 4 game from console to GUI.

## Quick Start

### Option 1: Automated Creation (Recommended)

Use the provided shell script to create all issues at once:

```bash
# Make sure you have GitHub CLI installed and authenticated
gh auth login

# Run the script
./create-issues.sh
```

This will create all 20 issues in the GitHub repository with proper labels.

### Option 2: Manual Creation

You can manually create issues by copying the content from each markdown file:

1. Go to https://github.com/chichha/connect4-cpp/issues/new
2. Open a task file (e.g., `task-1-1-gui-library-dependency.md`)
3. Copy the title from the frontmatter
4. Copy the body content (everything after the second `---`)
5. Add the labels specified in the frontmatter
6. Create the issue

### Option 3: GitHub CLI (One at a Time)

Create individual issues using the GitHub CLI:

```bash
# Example for Task 1.1
gh issue create \
  --repo chichha/connect4-cpp \
  --title "Task 1.1: Add GUI Library Dependency" \
  --body-file issues/task-1-1-gui-library-dependency.md \
  --label "enhancement,phase-1-setup"
```

## Task List

### Phase 1: Setup (2 tasks)
- `task-1-1-gui-library-dependency.md` - Add GUI Library Dependency
- `task-1-2-build-configuration.md` - Update Build Configuration

### Phase 2: Core UI (3 tasks)
- `task-2-1-window-rendering.md` - Create Window and Rendering Foundation
- `task-2-2-board-renderer.md` - Implement Board Renderer
- `task-2-3-piece-rendering.md` - Implement Game Piece Rendering

### Phase 3: User Interaction (3 tasks)
- `task-3-1-click-selection.md` - Implement Click-Based Column Selection
- `task-3-2-turn-feedback.md` - Add Visual Feedback for Player Turn
- `task-3-3-win-draw-display.md` - Implement Win/Draw Visual Display

### Phase 4: Game Controls (2 tasks)
- `task-4-1-new-game-button.md` - Add New Game / Reset Button
- `task-4-2-quit-button.md` - Add Quit/Exit Button

### Phase 5: Refactoring (3 tasks)
- `task-5-1-refactor-game-class.md` - Refactor Game Class for UI Independence
- `task-5-2-ui-manager.md` - Create UI Manager/Controller Class
- `task-5-3-update-main.md` - Update Main Entry Point

### Phase 6: Polish (4 tasks)
- `task-6-1-visual-polish.md` - Add Visual Polish
- `task-6-2-error-handling.md` - Handle Edge Cases and Errors
- `task-6-3-update-documentation.md` - Update Documentation
- `task-6-4-update-cicd.md` - Update CI/CD Pipeline

### Phase 7: Testing (2 tasks)
- `task-7-1-manual-testing.md` - Manual Testing
- `task-7-2-automated-testing.md` - Create Automated Tests (Optional)

## Labels Used

- `enhancement` - Feature enhancements
- `refactoring` - Code refactoring tasks
- `documentation` - Documentation updates
- `testing` - Testing tasks
- `design` - UI/UX design tasks
- `bug` - Bug fixes and error handling
- `ci/cd` - CI/CD pipeline tasks
- `optional` - Optional tasks
- `phase-1-setup` through `phase-7-testing` - Phase-specific labels

## File Format

Each task file follows this structure:

```markdown
---
title: "Task X.Y: Task Name"
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

## Estimated Effort
X-Y hours

## Phase
Phase N: Phase Name

## Dependencies
...

## Related Tasks
...
```

## Prerequisites

To use the automated script, you need:

1. **GitHub CLI** - Install from https://cli.github.com/
2. **Authentication** - Run `gh auth login` to authenticate
3. **Repository Access** - Write access to the repository

## Notes

- The script includes a 1-second delay between issue creations to avoid rate limiting
- All issues will be created with appropriate labels for easy filtering
- Issues are organized by phase for better project management
- Each issue includes dependencies and related tasks for context

## Troubleshooting

**"gh command not found"**
- Install GitHub CLI from https://cli.github.com/

**"Authentication failed"**
- Run `gh auth login` and follow the prompts

**"Permission denied"**
- Ensure you have write access to the repository

**Issues not created**
- Check the script output for specific error messages
- Verify the issue templates are valid markdown

## See Also

- [UI_MIGRATION_PLAN.md](../UI_MIGRATION_PLAN.md) - Detailed migration plan
- [TASK_SUMMARY.md](../TASK_SUMMARY.md) - Quick reference summary
