// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef IOS_API_H
#define IOS_API_H

#if defined(IOS_ENABLED)
extern void rebel_ios_plugins_initialize();
extern void rebel_ios_plugins_deinitialize();
#endif

void register_ios_api();
void unregister_ios_api();

#endif // IOS_API_H
