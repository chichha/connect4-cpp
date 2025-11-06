---
title: "Task 3.1: Implement Click-Based Column Selection"
labels: ["enhancement", "phase-3-interaction"]
assignees: []
---

## Description
Allow players to select columns by clicking on the board.

## Acceptance Criteria
- [ ] Mouse click events detected
- [ ] Click position correctly mapped to column (1-7)
- [ ] Invalid clicks (outside board) handled gracefully
- [ ] Visual feedback on column hover (highlight column)
- [ ] Only valid columns (not full) are selectable
- [ ] Column selection triggers piece drop

## Rationale
Replaces console input with intuitive mouse interaction.

## Estimated Effort
4-6 hours

## Phase
Phase 3: User Interaction

## Dependencies
- Task 2.2: Implement Board Renderer
- Task 2.3: Implement Game Piece Rendering

## Related Tasks
- Task 3.2: Add Visual Feedback for Player Turn
- Task 5.1: Refactor Game Class for UI Independence
