// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef BULLET_UTILITIES_H
#define BULLET_UTILITIES_H

/**
    @author AndreaCatania
*/

#define bulletnew(cl) new cl

#define bulletdelete(cl)                                                       \
    {                                                                          \
        delete cl;                                                             \
        cl = NULL;                                                             \
    }
#endif
