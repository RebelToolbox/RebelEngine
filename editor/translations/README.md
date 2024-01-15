# Rebel Editor Translations

This folder contains the Rebel Editor translation files that are synched with Weblate.

If you want to contribute to the Rebel Toolbox translations, please use our [Weblate](https://hosted.weblate.org/engage/rebel-toolbox/) project:
https://hosted.weblate.org/projects/rebel-toolbox/

## Rebel Editor Source Text Strings

The Rebel Editor source files use three tags to identify souce text strings:
- `TTR`: Text string only available when using the editor.
- `RTR`: Text string available in game and when using the editor.
- `TTRC`: C-strings

The `editor.pot` file contains all the extracted text strings enclosed in these tags.
The text strings in source files are extracted into the `editor.pot` file using `make update`.
`make update` runs the `tools/scripts/extract_editor_strings.py` script.

Each language has its own translation file; its `.po` file.
Updates to the `editor.pot` file are merged into each `.po` file using `make merge`.
The updating and merging tasks can be combined by running `make`.

## Rebel Editor Translations

The `editor.pot` file and all the `.po` files in this folder are synched with the Weblate project.
The Rebel Editor Weblate component is:
https://hosted.weblate.org/projects/rebel-toolbox/rebel-editor/

All updates to the translations are done on Weblate by the Weblate community.
Commits on Weblate are then synched with the `.po` files here.

If your language is not yet available, you can add a new language to the Weblate component.
New languages are added on Weblate:
https://hosted.weblate.org/new-lang/rebel-toolbox/rebel-editor/

For more information on Weblate and contributing to translations visit [Weblate](https://hosted.weblate.org/engage/rebel-toolbox/).
