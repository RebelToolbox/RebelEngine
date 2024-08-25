// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "bullet_types_converter.h"

// Bullet to Rebel
void B_TO_R(const btVector3& inVal, Vector3& outVal) {
    outVal[0] = inVal[0];
    outVal[1] = inVal[1];
    outVal[2] = inVal[2];
}

void INVERT_B_TO_R(const btVector3& inVal, Vector3& outVal) {
    outVal[0] = inVal[0] != 0. ? 1. / inVal[0] : 0.;
    outVal[1] = inVal[1] != 0. ? 1. / inVal[1] : 0.;
    outVal[2] = inVal[2] != 0. ? 1. / inVal[2] : 0.;
}

void B_TO_R(const btMatrix3x3& inVal, Basis& outVal) {
    B_TO_R(inVal[0], outVal[0]);
    B_TO_R(inVal[1], outVal[1]);
    B_TO_R(inVal[2], outVal[2]);
}

void INVERT_B_TO_R(const btMatrix3x3& inVal, Basis& outVal) {
    INVERT_B_TO_R(inVal[0], outVal[0]);
    INVERT_B_TO_R(inVal[1], outVal[1]);
    INVERT_B_TO_R(inVal[2], outVal[2]);
}

void B_TO_R(const btTransform& inVal, Transform& outVal) {
    B_TO_R(inVal.getBasis(), outVal.basis);
    B_TO_R(inVal.getOrigin(), outVal.origin);
}

// Rebel to Bullet
void R_TO_B(const Vector3& inVal, btVector3& outVal) {
    outVal[0] = inVal[0];
    outVal[1] = inVal[1];
    outVal[2] = inVal[2];
}

void INVERT_R_TO_B(const Vector3& inVal, btVector3& outVal) {
    outVal[0] = inVal[0] != 0. ? 1. / inVal[0] : 0.;
    outVal[1] = inVal[1] != 0. ? 1. / inVal[1] : 0.;
    outVal[2] = inVal[2] != 0. ? 1. / inVal[2] : 0.;
}

void R_TO_B(const Basis& inVal, btMatrix3x3& outVal) {
    R_TO_B(inVal[0], outVal[0]);
    R_TO_B(inVal[1], outVal[1]);
    R_TO_B(inVal[2], outVal[2]);
}

void INVERT_R_TO_B(const Basis& inVal, btMatrix3x3& outVal) {
    INVERT_R_TO_B(inVal[0], outVal[0]);
    INVERT_R_TO_B(inVal[1], outVal[1]);
    INVERT_R_TO_B(inVal[2], outVal[2]);
}

void R_TO_B(const Transform& inVal, btTransform& outVal) {
    R_TO_B(inVal.basis, outVal.getBasis());
    R_TO_B(inVal.origin, outVal.getOrigin());
}

void UNSCALE_BT_BASIS(btTransform& scaledBasis) {
    btMatrix3x3& basis(scaledBasis.getBasis());
    btVector3 column0 = basis.getColumn(0);
    btVector3 column1 = basis.getColumn(1);
    btVector3 column2 = basis.getColumn(2);

    // Check for zero scaling.
    if (column0.fuzzyZero()) {
        if (column1.fuzzyZero()) {
            if (column2.fuzzyZero()) {
                // All dimensions are fuzzy zero. Create a default basis.
                column0 = btVector3(1, 0, 0);
                column1 = btVector3(0, 1, 0);
                column2 = btVector3(0, 0, 1);
            } else { // Column 2 scale not fuzzy zero.
                // Create two vectors orthogonal to row 2.
                // Ensure that a default basis is created if row 2 = <0, 0, 1>
                column1 = btVector3(0, column2[2], -column2[1]);
                column0 = column1.cross(column2);
            }
        } else { // Column 1 scale not fuzzy zero.
            if (column2.fuzzyZero()) {
                // Create two vectors othogonal to column 1.
                // Ensure that a default basis is created if column 1 = <0, 1,
                // 0>
                column0 = btVector3(column1[1], -column1[0], 0);
                column2 = column0.cross(column1);
            } else { // Column 1 and column 2 scales not fuzzy zero.
                // Create column 0 orthogonal to column 1 and column 2.
                column0 = column1.cross(column2);
            }
        }
    } else { // Column 0 scale not fuzzy zero.
        if (column1.fuzzyZero()) {
            if (column2.fuzzyZero()) {
                // Create two vectors orthogonal to column 0.
                // Ensure that a default basis is created if column 0 = <1, 0,
                // 0>
                column2 = btVector3(-column0[2], 0, column0[0]);
                column1 = column2.cross(column0);
            } else { // Column 0 and column 2 scales not fuzzy zero.
                // Create column 1 orthogonal to column 0 and column 2.
                column1 = column2.cross(column0);
            }
        } else { // Column 0 and column 1 scales not fuzzy zero.
            if (column2.fuzzyZero()) {
                // Create column 2 orthogonal to column 0 and column 1.
                column2 = column0.cross(column1);
            }
        }
    }

    // Normalize
    column0.normalize();
    column1.normalize();
    column2.normalize();

    basis.setValue(
        column0[0],
        column1[0],
        column2[0],
        column0[1],
        column1[1],
        column2[1],
        column0[2],
        column1[2],
        column2[2]
    );
}
