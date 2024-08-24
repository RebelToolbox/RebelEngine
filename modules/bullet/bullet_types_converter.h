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

// Bullet to Rebel
extern void B_TO_R(const btVector3& inVal, Vector3& outVal);
extern void INVERT_B_TO_R(const btVector3& inVal, Vector3& outVal);
extern void B_TO_R(const btMatrix3x3& inVal, Basis& outVal);
extern void INVERT_B_TO_R(const btMatrix3x3& inVal, Basis& outVal);
extern void B_TO_R(const btTransform& inVal, Transform& outVal);

// Rebel to Bullet
extern void R_TO_B(const Vector3& inVal, btVector3& outVal);
extern void INVERT_R_TO_B(const Vector3& inVal, btVector3& outVal);
extern void R_TO_B(const Basis& inVal, btMatrix3x3& outVal);
extern void INVERT_R_TO_B(const Basis& inVal, btMatrix3x3& outVal);
extern void R_TO_B(const Transform& inVal, btTransform& outVal);

extern void UNSCALE_BT_BASIS(btTransform& scaledBasis);

#endif // BULLET_TYPES_CONVERTER_H
