---
title: "Task 5.1: Refactor Game Class for UI Independence"
labels: ["enhancement", "refactoring", "phase-5-refactoring"]
assignees: []
---

## Description
Separate game logic from UI concerns (following MVC or similar pattern).

## Acceptance Criteria
- [ ] `Game` class does not directly handle console I/O
- [ ] Game state queryable by UI layer
- [ ] Game actions invokable by UI events
- [ ] Board data accessible for rendering
- [ ] Unit tests verify logic still works (if tests exist)
- [ ] Console version still functional (if maintained)

## Rationale
Maintainability and potential for multiple UI frontends.

## Estimated Effort
4-6 hours

## Phase
Phase 5: Refactoring and Integration

## Dependencies
- Task 3.1: Implement Click-Based Column Selection

## Related Tasks
- Task 5.2: Create UI Manager/Controller Class
