// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifdef TOOLS_ENABLED

#include "math_fieldwise.h"

#define SETUP_TYPE(m_type)                                                     \
    m_type source = p_source;                                                  \
    m_type target = p_target;
#define TRY_TRANSFER_FIELD(m_name, m_member)                                   \
    if (p_field == m_name) {                                                   \
        target.m_member = source.m_member;                                     \
    }

Variant fieldwise_assign(
    const Variant& p_target,
    const Variant& p_source,
    const String& p_field
) {
    ERR_FAIL_COND_V(p_target.get_type() != p_source.get_type(), p_target);

    switch (p_source.get_type()) {
        case Variant::VECTOR2: {
            SETUP_TYPE(Vector2)

            /**/ TRY_TRANSFER_FIELD("x", x)
            else TRY_TRANSFER_FIELD("y", y)

                return target;
        }

        case Variant::RECT2: {
            SETUP_TYPE(Rect2)

            /**/ TRY_TRANSFER_FIELD("x", position.x)
            else TRY_TRANSFER_FIELD("y", position.y) else TRY_TRANSFER_FIELD(
                "w",
                size.x
            ) else TRY_TRANSFER_FIELD("h", size.y)

                return target;
        }

        case Variant::VECTOR3: {
            SETUP_TYPE(Vector3)

            /**/ TRY_TRANSFER_FIELD("x", x)
            else TRY_TRANSFER_FIELD("y", y) else TRY_TRANSFER_FIELD("z", z)

                return target;
        }

        case Variant::PLANE: {
            SETUP_TYPE(Plane)

            /**/ TRY_TRANSFER_FIELD("x", normal.x)
            else TRY_TRANSFER_FIELD("y", normal.y) else TRY_TRANSFER_FIELD(
                "z",
                normal.z
            ) else TRY_TRANSFER_FIELD("d", d)

                return target;
        }

        case Variant::QUAT: {
            SETUP_TYPE(Quat)

            /**/ TRY_TRANSFER_FIELD("x", x)
            else TRY_TRANSFER_FIELD("y", y) else TRY_TRANSFER_FIELD(
                "z",
                z
            ) else TRY_TRANSFER_FIELD("w", w)

                return target;
        }

        case Variant::AABB: {
            SETUP_TYPE(AABB)

            /**/ TRY_TRANSFER_FIELD("px", position.x)
            else TRY_TRANSFER_FIELD("py", position.y)
			else TRY_TRANSFER_FIELD("pz", position.z)
			else TRY_TRANSFER_FIELD("sx", size.x)
			else TRY_TRANSFER_FIELD("sy", size.y)
			else TRY_TRANSFER_FIELD("sz", size.z)

			return target;
        }

        case Variant::TRANSFORM2D: {
            SETUP_TYPE(Transform2D)

            /**/ TRY_TRANSFER_FIELD("xx", elements[0][0])
            else TRY_TRANSFER_FIELD("xy", elements[0][1])
			else TRY_TRANSFER_FIELD("yx", elements[1][0])
			else TRY_TRANSFER_FIELD("yy", elements[1][1])
			else TRY_TRANSFER_FIELD("ox", elements[2][0])
			else TRY_TRANSFER_FIELD("oy", elements[2][1])

			return target;
        }

        case Variant::BASIS: {
            SETUP_TYPE(Basis)

            /**/ TRY_TRANSFER_FIELD("xx", elements[0][0])
            else TRY_TRANSFER_FIELD("xy", elements[0][1])
			else TRY_TRANSFER_FIELD("xz", elements[0][2])
			else TRY_TRANSFER_FIELD("yx", elements[1][0])
			else TRY_TRANSFER_FIELD("yy", elements[1][1])
			else TRY_TRANSFER_FIELD("yz", elements[1][2])
			else TRY_TRANSFER_FIELD("zx", elements[2][0])
			else TRY_TRANSFER_FIELD("zy", elements[2][1])
			else TRY_TRANSFER_FIELD("zz", elements[2][2])

			return target;
        }

        case Variant::TRANSFORM: {
            SETUP_TYPE(Transform)

            /**/ TRY_TRANSFER_FIELD("xx", basis.elements[0][0])
            else TRY_TRANSFER_FIELD("xy", basis.elements[0][1])
			else TRY_TRANSFER_FIELD("xz", basis.elements[0][2])
			else TRY_TRANSFER_FIELD("yx", basis.elements[1][0])
			else TRY_TRANSFER_FIELD("yy", basis.elements[1][1])
			else TRY_TRANSFER_FIELD("yz", basis.elements[1][2])
			else TRY_TRANSFER_FIELD("zx", basis.elements[2][0])
			else TRY_TRANSFER_FIELD("zy", basis.elements[2][1])
			else TRY_TRANSFER_FIELD("zz", basis.elements[2][2])
			else TRY_TRANSFER_FIELD("xo", origin.x)
			else TRY_TRANSFER_FIELD("yo", origin.y)
			else TRY_TRANSFER_FIELD("zo", origin.z)

			return target;
        }

        default: {
            ERR_FAIL_V(p_target);
        }
    }
}

#endif // TOOLS_ENABLED
