// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef OCTREE_H
#define OCTREE_H

#define OCTREE_ELEMENT_INVALID_ID   0
#define OCTREE_SIZE_LIMIT           1e15
#define OCTREE_DEFAULT_OCTANT_LIMIT 0

// We want 2 versions of the octree, Octree
// and Octree_CL which uses cached lists (optimized).
// we don't want to use the extra memory of cached lists on
// the non cached list version, so we use macros
// to avoid duplicating the code which is in octree_definition.
// The name of the class is overridden and the changes with the define
// OCTREE_USE_CACHED_LISTS.

// The two classes can be used identically but one contains the cached
// list optimization.

// standard octree
#define OCTREE_CLASS_NAME Octree
#undef OCTREE_USE_CACHED_LISTS
#include "octree_definition.inc"
#undef OCTREE_CLASS_NAME

// cached lists octree
#define OCTREE_CLASS_NAME Octree_CL
#define OCTREE_USE_CACHED_LISTS
#include "octree_definition.inc"
#undef OCTREE_CLASS_NAME

#endif // OCTREE_H
