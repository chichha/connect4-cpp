# GitHub Action: Create UI Migration Issues

This workflow automatically creates all 20 UI migration task issues in the repository.

## How to Use

### Option 1: GitHub Actions (Recommended)

1. Go to the **Actions** tab in your repository: https://github.com/chichha/connect4-cpp/actions
2. Select the **"Create UI Migration Issues"** workflow from the left sidebar
3. Click the **"Run workflow"** button (on the right side)
4. Type `create-issues` in the confirmation field
5. Click **"Run workflow"** to start the process

The workflow will:
- ✅ Create all 20 issues with proper titles, descriptions, and labels
- ✅ Add acceptance criteria checklists to each issue
- ✅ Link related tasks and dependencies
- ✅ Apply phase-based labels for easy filtering

**Important:** The confirmation input is required to prevent accidental execution.

### Option 2: Local Script

If you prefer to run the script locally:

```bash
# Authenticate with GitHub CLI
gh auth login

# Run the creation script
./create-issues.sh
```

## What Gets Created

The workflow creates **20 GitHub issues** organized across **7 phases**:

### Phase 1: Setup (2 issues)
- Task 1.1: Add GUI Library Dependency
- Task 1.2: Update Build Configuration

### Phase 2: Core UI (3 issues)
- Task 2.1: Create Window and Rendering Foundation
- Task 2.2: Implement Board Renderer
- Task 2.3: Implement Game Piece Rendering

### Phase 3: User Interaction (3 issues)
- Task 3.1: Implement Click-Based Column Selection
- Task 3.2: Add Visual Feedback for Player Turn
- Task 3.3: Implement Win/Draw Visual Display

### Phase 4: Game Controls (2 issues)
- Task 4.1: Add New Game / Reset Button
- Task 4.2: Add Quit/Exit Button

### Phase 5: Refactoring (3 issues)
- Task 5.1: Refactor Game Class for UI Independence
- Task 5.2: Create UI Manager/Controller Class
- Task 5.3: Update Main Entry Point

### Phase 6: Polish (4 issues)
- Task 6.1: Add Visual Polish
- Task 6.2: Handle Edge Cases and Errors
- Task 6.3: Update Documentation
- Task 6.4: Update CI/CD Pipeline

### Phase 7: Testing (2 issues)
- Task 7.1: Manual Testing
- Task 7.2: Create Automated Tests (Optional)

## Labels Applied

Each issue is automatically labeled with:
- **Type labels**: `enhancement`, `refactoring`, `documentation`, `testing`, `design`, `bug`, `ci/cd`
- **Phase labels**: `phase-1-setup`, `phase-2-core-ui`, `phase-3-interaction`, `phase-4-controls`, `phase-5-refactoring`, `phase-6-polish`, `phase-7-testing`
- **Special labels**: `optional` (for Task 7.2)

## Permissions

The workflow requires the following permissions:
- `issues: write` - To create new issues
- `contents: read` - To access the repository files

These are configured in the workflow file and use the built-in `GITHUB_TOKEN`.

## Troubleshooting

### "Confirmation input must be 'create-issues'"
- Make sure you type exactly `create-issues` (without quotes) in the confirmation field

### Issues already exist
- The workflow will attempt to create all issues even if some already exist
- GitHub will reject duplicates, which is safe
- Check the workflow logs to see which issues were created successfully

### Permission denied
- Ensure the repository has Issues enabled (Settings → Features → Issues)
- Check that the workflow has the correct permissions in the YAML file

## Technical Details

- **Workflow file**: `.github/workflows/create-issues.yml`
- **Creation script**: `create-issues.sh`
- **Issue templates**: `issues/task-*.md` (20 files)
- **Trigger**: Manual workflow dispatch only (prevents accidental runs)
- **Rate limiting**: 1-second delay between issue creations to avoid API limits

## After Creation

Once issues are created:
1. Review all issues at: https://github.com/chichha/connect4-cpp/issues
2. Filter by phase using labels (e.g., `label:phase-1-setup`)
3. Assign issues to team members
4. Create a project board to track progress
5. Link issues to pull requests as work progresses

## See Also

- [UI_MIGRATION_PLAN.md](../UI_MIGRATION_PLAN.md) - Detailed migration plan
- [TASK_SUMMARY.md](../TASK_SUMMARY.md) - Quick reference checklist
- [issues/README.md](../issues/README.md) - Issue templates documentation
