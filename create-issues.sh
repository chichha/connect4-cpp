#!/bin/bash

# Script to create GitHub issues from the task markdown files
# This script uses the GitHub CLI (gh) to create issues

REPO="chichha/connect4-cpp"
ISSUES_DIR="./issues"

# Check if gh CLI is installed
if ! command -v gh &> /dev/null; then
    echo "Error: GitHub CLI (gh) is not installed."
    echo "Please install it from: https://cli.github.com/"
    exit 1
fi

# Check if authenticated
if ! gh auth status &> /dev/null; then
    echo "Error: Not authenticated with GitHub CLI."
    echo "Please run: gh auth login"
    exit 1
fi

echo "Creating issues for repository: $REPO"
echo "================================================"
echo ""

# Counter for created issues
created=0
failed=0

# Process each markdown file in the issues directory
for file in "$ISSUES_DIR"/task-*.md; do
    if [ -f "$file" ]; then
        filename=$(basename "$file")
        echo "Processing: $filename"
        
        # Extract title from frontmatter
        title=$(grep "^title:" "$file" | sed 's/title: *"\(.*\)"/\1/')
        
        # Extract labels from frontmatter
        labels=$(grep "^labels:" "$file" | sed 's/labels: *\[\(.*\)\]/\1/' | tr -d '"' | tr ',' '\n' | xargs)
        
        # Remove frontmatter and get body
        body=$(sed '1,/^---$/d' "$file" | sed '1,/^---$/d')
        
        # Create the issue
        if gh issue create --repo "$REPO" --title "$title" --body "$body" --label "$labels" > /dev/null 2>&1; then
            echo "  ✓ Created issue: $title"
            ((created++))
        else
            echo "  ✗ Failed to create issue: $title"
            ((failed++))
        fi
        
        echo ""
        
        # Add a small delay to avoid rate limiting
        sleep 1
    fi
done

echo "================================================"
echo "Summary:"
echo "  Created: $created issues"
echo "  Failed:  $failed issues"
echo "================================================"

if [ $created -gt 0 ]; then
    echo ""
    echo "View all issues at: https://github.com/$REPO/issues"
fi
