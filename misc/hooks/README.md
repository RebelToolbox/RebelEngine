# Git hooks for Rebel Engine

Hooks are `bash` scripts that are run when calling `git commit`.
These hooks help Rebel Engine contributors comply with the Rebel Engine style guides.

## Installation

Ensure that
[`clang-format`](https://clang.llvm.org/docs/ClangFormat.html)
and
[`black`](https://pypi.org/project/black/)
are installed and available in the system `path`.
Copy all the files from this folder into your `.git/hooks` folder.
Confirm that all the scripts are executable.

## Hook Details

## `pre-commit`

`pre-commit` is the hook called by Git when calling `git commit`.
It calls all the other hooks.
Additional hooks can be added by updating the `hooks` variable.
There are currently three hooks:
- pre-commit-clang-format
- pre-commit-black
- pre-commit-make-rst

## `pre-commit-clang-format`

`pre-commit-clang-format` applies
[`clang-format`](https://clang.llvm.org/docs/ClangFormat.html)
to all code files.
The `clang-format` rules are defined in `.clang-format` in the Rebel Engine root directory.
The version of `clang-format` used affects the output generated.
If there is a difference, use the version used by CI: currently version 17.

## `pre-commit-black`

`pre-commit-black` applies the
[`black`](https://pypi.org/project/black/)
Python style checks to all Python and SCons files.

## `pre-commit-make-rst`

`pre-commit-make-rst` checks the class reference syntax.
`pre-commit-make-rst` performs a dry run of `doc/tools/make_rst.py`.

## `pre-commit-style-check`

To standardise the style checks,
both `pre-commit-clang-format` and `pre-commit-black` call `pre-commit-style-check`.

`pre-commit-style-check` takes six optional parameters:
- `application_name`: The style check application to run.
- `options`: The options passed to the style check application.
- `include_files`: A list of specific filenames to be included.
- `include_extensions`: A list of filename extensions to be included.
- `exclude_directories`: A list of directories to be excluded.
- `exclude_file_patterns`: A list of file patterns to be exclued.

If the changes do not comply with the style check requirements, the commit is aborted.
However, a patch file is created that includes the changes needed to comply with the style check.
You are prompted to apply, apply and stage or ignore the required changes.
`git commit` needs to be run again; either with the required changes applied (and staged), or
without the required changes (not recommended) by using `git commit --no-verify`.

## `messagebox.ps1`
`messagebox.ps1` is a
[Powershell](https://learn.microsoft.com/en-gb/powershell/)
script.
When `git commit` is not run from an interactive terminal and
changes do not comply with the style check requirements
(or the required style check applications are not found)
`pre-commit-style-check` interacts with you via a window.
On Windows, this is done via `messagebox.ps1`.

`messagebox.ps1` takes six optional parameters:
- `title`: The message box's title.
- `message`: If specified, the message to be displayed.
- `file`: If specified, displays the contents of the specified file below the message.
- `buttons`: The message box's buttons separated by commas, and codes separated by a colon.
- `size`: The message box's width and height separated by an 'x' e.g. "400x200".
- `icon`: The message box's icon file.
