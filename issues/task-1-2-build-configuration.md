---
title: "Task 1.2: Update Build Configuration"
labels: ["enhancement", "phase-1-setup"]
assignees: []
---

## Description
Configure build system to support both console and GUI builds (optional: make GUI the default).

## Acceptance Criteria
- [ ] CMake option to toggle between console/GUI builds (optional)
- [ ] Proper compiler flags for GUI applications set
- [ ] Windows subsystem set to WINDOWS (not CONSOLE) for release builds
- [ ] Build produces executable with correct linking

## Rationale
Allows gradual migration and testing of both versions during development.

## Estimated Effort
1-2 hours

## Phase
Phase 1: Project Setup and Infrastructure

## Dependencies
- Task 1.1: Add GUI Library Dependency

## Related Tasks
- Task 2.1: Create Window and Rendering Foundation
