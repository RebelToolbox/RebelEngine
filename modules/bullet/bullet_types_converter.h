// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef BULLET_TYPES_CONVERTER_H
#define BULLET_TYPES_CONVERTER_H

#include "core/math/basis.h"
#include "core/math/transform.h"
#include "core/math/vector3.h"
#include "core/typedefs.h"

#include <LinearMath/btMatrix3x3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>

/**
    @author AndreaCatania
*/

// Bullet to Godot
extern void B_TO_G(btVector3 const& inVal, Vector3& outVal);
extern void INVERT_B_TO_G(btVector3 const& inVal, Vector3& outVal);
extern void B_TO_G(btMatrix3x3 const& inVal, Basis& outVal);
extern void INVERT_B_TO_G(btMatrix3x3 const& inVal, Basis& outVal);
extern void B_TO_G(btTransform const& inVal, Transform& outVal);

// Godot TO Bullet
extern void G_TO_B(Vector3 const& inVal, btVector3& outVal);
extern void INVERT_G_TO_B(Vector3 const& inVal, btVector3& outVal);
extern void G_TO_B(Basis const& inVal, btMatrix3x3& outVal);
extern void INVERT_G_TO_B(Basis const& inVal, btMatrix3x3& outVal);
extern void G_TO_B(Transform const& inVal, btTransform& outVal);

extern void UNSCALE_BT_BASIS(btTransform& scaledBasis);
#endif
