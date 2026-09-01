#!/usr/bin/env bash

# This script ensures proper POSIX text file formatting and a few other things.
# This is supplementary to clang_format.sh and black_format.sh, but should be
# run before them.

# We need recode and dos2unix.
if [ ! -x "$(command -v recode)" -o ! -x "$(command -v dos2unix)" ]; then
    echo "Install 'recode' and 'dos2unix' to use this script."
    echo "::error::'recode' and 'dos2unix' are required to perform file formatting checks." && exit 1
fi

set -uo pipefail
IFS=$'\n\t'

# Loops through all text files tracked by Git.
git grep -zIl '' |
while IFS= read -rd '' f; do
    # Exclude some files.
    if [[ "$f" == *"csproj" ]]; then
        continue
    elif [[ "$f" == *"sln" ]]; then
        continue
    elif [[ "$f" == *".bat" ]]; then
        continue
    elif [[ "$f" == *"patch" ]]; then
        continue
    elif [[ "$f" == *"pot" ]]; then
        continue
    elif [[ "$f" == *"po" ]]; then
        continue
    elif [[ "$f" == "third-party"* ]]; then
        continue
    elif [[ "$f" == "platforms/android/project/engine/src/main/java/com/google"* ]]; then
        continue
    elif [[ "$f" == *"-so_wrap."* ]]; then
        continue
    fi
    # Ensure that files are UTF-8 formatted.
    recode UTF-8 "$f" 2> /dev/null
    # Ensure that files have LF line endings and do not contain a BOM.
    dos2unix "$f" 2> /dev/null
    # Remove trailing space characters and ensures that files end
    # with newline characters. -l option handles newlines conveniently.
    perl -i -ple 's/\s*$//g' "$f"
    # Remove the character sequence "== true" if it has a leading space.
    perl -i -pe 's/\x20== true//g' "$f"
done

# If a diff has been created, notify the user and exit with an error.
if [[ $(git diff) ]]; then
    echo "Files in this commit do not comply with the file formatting rules."
    echo "The following differences were found between the code and the formatting rules:"
    echo
    git diff --color
    echo
    echo "Please fix your commit(s)"
    echo "::error::Files in this commit do not comply with the file formatting rules." && exit 1
fi

echo "Files in this commit comply with the file formatting rules."
