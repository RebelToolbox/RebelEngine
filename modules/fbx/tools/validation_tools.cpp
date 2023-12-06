// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "validation_tools.h"

#ifdef TOOLS_ENABLED

#include "core/print_string.h"
#include "core/ustring.h"

ValidationTracker::Entries* ValidationTracker::entries_singleton =
    memnew(ValidationTracker::Entries);

// for printing our CSV to dump validation problems of files
// later we can make some agnostic tooling for this but this is fine for the
// time being.
void ValidationTracker::Entries::add_validation_error(
    String asset_path,
    String message
) {
    print_error(message);
    // note: implementation is static
    validation_entries[asset_path].push_back(message);
}

#endif // TOOLS_ENABLED
