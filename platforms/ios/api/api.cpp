// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "api.h"

#if defined(IOS_ENABLED)

void register_ios_api() {
    rebel_ios_plugins_initialize();
}

void unregister_ios_api() {
    rebel_ios_plugins_deinitialize();
}

#else

void register_ios_api() {}

void unregister_ios_api() {}

#endif
