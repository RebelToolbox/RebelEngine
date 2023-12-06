// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifdef __linux__
#include <alloca.h>
#endif

#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
#include <stdlib.h> // alloca
// FreeBSD and OpenBSD use pthread_set_name_np, while other platforms,
// include NetBSD, use pthread_setname_np. NetBSD's version however requires
// a different format, we handle this directly in thread_posix.
#ifdef __NetBSD__
#define PTHREAD_NETBSD_SET_NAME
#else
#define PTHREAD_BSD_SET_NAME
#endif
#endif

#define GLES3_INCLUDE_H "thirdparty/glad/glad/glad.h"
#define GLES2_INCLUDE_H "thirdparty/glad/glad/glad.h"
