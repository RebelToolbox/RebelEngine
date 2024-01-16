# Rebel Engine API Translations

This folder contains the Rebel Engine API translation files that are synched with Weblate.

If you want to contribute to the Rebel Toolbox translations, please use our [Weblate](https://hosted.weblate.org/engage/rebel-toolbox/) project:
https://hosted.weblate.org/projects/rebel-toolbox/

## Rebel Engine API Source Text Strings

The Rebel Engine API source files are the XML files in the [`docs`](https://github.com/RebelToolbox/RebelEngine/tree/main/docs) folder.
Rebel Engine also has a number of included modules listed under the [`modules`](https://github.com/RebelToolbox/RebelEngine/tree/main/modules) folder.
Modules that extend the API have their own `docs` folder with additional API XML files.
The Rebel Engine API text strings to be translated are extracted from all these API XML files.

The `api.pot` file contains all the extracted API XML files' text strings.
The text strings in the XML files are extracted into the `api.pot` file using `make update`.
`make update` runs the `tools/scripts/extract_api_strings.py` script.
`make update` ensures that the script scans the `docs` folder and each modules' `docs` folder.

Each language has its own translation file; its `.po` file.
Updates to the `api.pot` file are merged into each `.po` file using `make merge`.
The updating and merging tasks can be combined by running `make`.

## Rebel Engine API Translations

The `api.pot` file and all the `.po` files in this folder are synched with the Weblate project.
The Rebel Engine API Weblate component is:
https://hosted.weblate.org/projects/rebel-toolbox/rebel-engine-api/

All updates to the translations are done on Weblate by the Weblate community.
Commits on Weblate are then synched with the `.po` files here.

If your language is not yet available, you can add a new language to the Weblate component.
New languages are added on Weblate:
https://hosted.weblate.org/new-lang/rebel-toolbox/rebel-engine-api/

For more information on Weblate and contributing to translations visit [Weblate](https://hosted.weblate.org/engage/rebel-toolbox/).
