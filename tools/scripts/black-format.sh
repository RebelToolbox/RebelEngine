#!/usr/bin/env bash

# This script runs black on all Python files in the repo.

set -uo pipefail

# Apply black.
echo -e "Formatting Python files..."
PY_FILES=$(find \( -path "./.git" \
                -o -path "./third-party" \
                \) -prune \
                -o \( -name "SConstruct" \
                -o -name "SCsub" \
                -o -name "*.py" \
                \) -print)
black $PY_FILES

# If a diff has been created, notify the user and exit with error.
if [[ $(git diff) ]]; then
    echo "Files in this commit do not comply with the black Python style rules."
    echo "The following differences were found between the code and the formatting rules:"
    echo
    git diff --color
    echo
    echo "Please fix your commit(s)"

    echo "::error::Files in this commit do not comply with the black Python style rules." && exit 1
fi

echo "Files in this commit comply with the black Python style rules."
