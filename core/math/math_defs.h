// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef MATH_DEFS_H
#define MATH_DEFS_H

#define CMP_EPSILON  0.00001
#define CMP_EPSILON2 (CMP_EPSILON * CMP_EPSILON)

#define CMP_NORMALIZE_TOLERANCE    0.000001
#define CMP_POINT_IN_PLANE_EPSILON 0.00001

#define Math_SQRT12 0.7071067811865475244008443621048490
#define Math_SQRT2  1.4142135623730950488016887242
#define Math_LN2    0.6931471805599453094172321215
#define Math_TAU    6.2831853071795864769252867666
#define Math_PI     3.1415926535897932384626433833
#define Math_E      2.7182818284590452353602874714
#define Math_INF    INFINITY
#define Math_NAN    NAN

#ifdef DEBUG_ENABLED
#define MATH_CHECKS
#endif

// this epsilon is for values related to a unit size (scalar or vector len)
#ifdef PRECISE_MATH_CHECKS
#define UNIT_EPSILON 0.00001
#else
// tolerate some more floating point error normally
#define UNIT_EPSILON 0.001
#endif

#define USEC_TO_SEC(m_usec) ((m_usec) / 1000000.0)

enum ClockDirection {
    CLOCKWISE,
    COUNTERCLOCKWISE
};

enum Orientation {
    HORIZONTAL,
    VERTICAL
};

enum HAlign {
    HALIGN_LEFT,
    HALIGN_CENTER,
    HALIGN_RIGHT
};

enum VAlign {
    VALIGN_TOP,
    VALIGN_CENTER,
    VALIGN_BOTTOM
};

enum Margin {
    MARGIN_LEFT,
    MARGIN_TOP,
    MARGIN_RIGHT,
    MARGIN_BOTTOM
};

enum Corner {
    CORNER_TOP_LEFT,
    CORNER_TOP_RIGHT,
    CORNER_BOTTOM_RIGHT,
    CORNER_BOTTOM_LEFT
};

/**
 * The "Real" type is an abstract type used for real numbers, such as 1.5,
 * in contrast to integer numbers. Precision can be controlled with the
 * presence or absence of the REAL_T_IS_DOUBLE define.
 */
#ifdef REAL_T_IS_DOUBLE
typedef double real_t;
#else
typedef float real_t;
#endif

#endif // MATH_DEFS_H
