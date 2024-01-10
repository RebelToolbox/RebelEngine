#!/usr/bin/env bash

# This script runs clang-format and fixes copyright headers on all relevant files in the repo.
# This is the primary script responsible for fixing style violations.

set -uo pipefail
IFS=$'\n\t'

CLANG_FORMAT_FILE_EXTS=(".c" ".h" ".cpp" ".hpp" ".cc" ".hh" ".cxx" ".m" ".mm" ".inc" ".java" ".glsl")

# Loops through all text files tracked by Git.
git grep -zIl '' |
while IFS= read -rd '' f; do
    # Exclude some files.
    if [[ "$f" == "thirdparty"* ]]; then
        continue
    elif [[ "$f" == "platform/android/java/lib/src/com/google"* ]]; then
        continue
    elif [[ "$f" == *"-so_wrap."* ]]; then
        continue
    fi

    for extension in ${CLANG_FORMAT_FILE_EXTS[@]}; do
        if [[ "$f" == *"$extension" ]]; then
            # Run clang-format.
            clang-format --Wno-error=unknown -i "$f"
            continue 2
        fi
    done
done

# If a diff has been created, notify the user and exit with an error.
if [[ $(git diff) ]]; then
    echo "Files in this commit do not comply with the clang-format style rules."
    echo "The following differences were found between the code and the formatting rules:"
    echo
    git diff --color
    echo
    echo "Please fix your commit(s)"
    echo "::error::Files in this commit do not comply with the clang-format style rules." && exit 1
fi

echo "Files in this commit comply with the clang-format style rules."
