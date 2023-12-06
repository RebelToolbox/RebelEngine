// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef IPHONE_API_H
#define IPHONE_API_H

#if defined(IPHONE_ENABLED)
extern void godot_ios_plugins_initialize();
extern void godot_ios_plugins_deinitialize();
#endif

void register_iphone_api();
void unregister_iphone_api();

#endif // IPHONE_API_H
