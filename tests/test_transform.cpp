// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#include "test_transform.h"

#include "core/math/random_number_generator.h"
#include "core/math/transform.h"
#include "core/math/vector3.h"
#include "core/os/os.h"
#include "core/ustring.h"

namespace TestTransform {

bool test_plane() {
    bool pass = true;

    // test non-uniform scaling, forward and inverse
    Transform tr;
    tr.scale(Vector3(1, 2, 3));

    Plane p(Vector3(1, 1, 1), Vector3(1, 1, 1).normalized());

    Plane p2 = tr.xform(p);
    Plane p3 = tr.xform_inv(p2);

    if (!p3.normal.is_equal_approx(p.normal)) {
        OS::get_singleton()->print("Fail due to Transform::xform(Plane)\n");
        pass = false;
    }

    return pass;
}

bool test_aabb_regular() {
    bool pass = true;

    Transform tr;
    tr.basis  = Basis(Vector3(static_cast<real_t>(Math_PI), 0, 0));
    tr.origin = Vector3(1, 2, 3);

    AABB bb(Vector3(1, 1, 1), Vector3(2, 3, 4));

    // Test forward xform.
    AABB bb2 = tr.xform(bb);
    AABB bb3 = tr.xform_inv(bb2);

    if (!bb3.position.is_equal_approx(bb.position)) {
        OS::get_singleton()->print(
            "Fail due to Transform::xform_inv(AABB) position\n"
        );
        pass = false;
    }

    if (!bb3.size.is_equal_approx(bb.size)) {
        OS::get_singleton()->print(
            "Fail due to Transform::xform_inv(AABB) size\n"
        );
        pass = false;
    }

    if (!pass) {
        String string = String("bb2 : ") + String(Variant(bb2));
        OS::get_singleton()->print("\t%ls\n", string.c_str());
        string = String("bb3 : ") + String(Variant(bb3));
        OS::get_singleton()->print("\t%ls\n", string.c_str());
    }

    return pass;
}

bool test_aabb_non_uniform_scale() {
    bool pass = true;

    Transform tr;
    tr.scale(Vector3(1, 2, 3));

    AABB bb(Vector3(1, 1, 1), Vector3(2, 3, 4));

    // Test forward xform.
    AABB bb2 = tr.xform(bb);

    if (!bb2.position.is_equal_approx(Vector3(1, 2, 3))) {
        OS::get_singleton()->print(
            "Fail with non-uniform scale due to Transform::xform(AABB) "
            "position\n"
        );
        pass = false;
    }

    if (!bb2.size.is_equal_approx(Vector3(2, 6, 12))) {
        OS::get_singleton()->print(
            "Fail with non-uniform scale due to Transform::xform(AABB) size\n"
        );
        pass = false;
    }

    // Now test inverse.
    // This will fail if using the transpose and not the affine_inverse.
    bb2.position = Vector3(1, 2, 3);
    bb2.size     = Vector3(2, 6, 12);

    AABB bb3 = tr.xform_inv(bb2);

    if (!bb3.position.is_equal_approx(bb.position)) {
        OS::get_singleton()->print(
            "Fail with non-uniform scale due to Transform::xform_inv(AABB) "
            "position\n"
        );
        pass = false;
    }

    if (!bb3.size.is_equal_approx(bb.size)) {
        OS::get_singleton()->print(
            "Fail with non-uniform scale due to Transform::xform_inv(AABB) "
            "size\n"
        );
        pass = false;
    }

    if (!pass) {
        String string = String("bb2 : ") + String(Variant(bb2));
        OS::get_singleton()->print("\t%ls\n", string.c_str());
        string = String("bb3 : ") + String(Variant(bb3));
        OS::get_singleton()->print("\t%ls\n", string.c_str());
    }

    return pass;
}

bool test_aabb() {
    bool pass = true;
    if (!test_aabb_regular()) {
        pass = false;
    }
    return pass;
}

bool test_vector3_regular() {
    bool pass = true;

    Transform tr;

    RandomNumberGenerator rng;
    const real_t range     = 1800.0;
    const real_t range_rot = static_cast<real_t>(Math_PI);

    bool passed_multi = true;
    for (int n = 0; n < 1000; n++) {
        Vector3 pt_test = Vector3(
            rng.randf_range(-range, range),
            rng.randf_range(-range, range),
            rng.randf_range(-range, range)
        );

        tr.origin = Vector3(
            rng.randf_range(-range, range),
            rng.randf_range(-range, range),
            rng.randf_range(-range, range)
        );
        tr.basis = Basis(Vector3(
            rng.randf_range(-range_rot, range_rot),
            rng.randf_range(-range_rot, range_rot),
            rng.randf_range(-range_rot, range_rot)
        ));

        Vector3 pt = tr.xform(pt_test);
        pt         = tr.xform_inv(pt);

        if (!pt.is_equal_approx(pt_test, 0.1f)) {
            passed_multi = false;
        }
    }
    if (!passed_multi) {
        OS::get_singleton()->print(
            "Failed multitest due to Transform::xform and xform_inv(Vector3)\n"
        );
        pass = false;
    }

    return pass;
}

bool test_vector3_non_uniform_scale() {
    bool pass = true;

    // Regular scale.
    Transform tr;
    tr.scale(Vector3(3, 3, 3));
    Vector3 pt(1, 1, 1);
    Vector3 res = tr.xform(pt);

    if (!res.is_equal_approx(Vector3(3, 3, 3))) {
        OS::get_singleton()->print(
            "Fail with scale due to Transform::xform(Vector3)\n"
        );
        pass = false;
    }

    res = tr.xform_inv(res);
    if (!res.is_equal_approx(pt)) {
        OS::get_singleton()->print(
            "Fail with scale due to Transform::xform_inv(Vector3)\n"
        );
        pass = false;
    }

    // Non uniform scale.
    tr.scale(Vector3(1, 2, 3));
    res = tr.xform(pt);

    if (!res.is_equal_approx(Vector3(1, 2, 3))) {
        OS::get_singleton()->print(
            "Fail with non-uniform scale due to Transform::xform(Vector3)\n"
        );
        pass = false;
    }

    pt  = Vector3(1, 2, 3);
    res = tr.xform_inv(pt);
    if (!res.is_equal_approx(Vector3(1, 1, 1))) {
        OS::get_singleton()->print(
            "Fail with non-uniform scale due to Transform::xform_inv(Vector3)\n"
        );
        pass = false;
    }

    return pass;
}

bool test_vector3() {
    bool pass = true;
    if (!test_vector3_regular()) {
        pass = false;
    }
    return pass;
}

MainLoop* test() {
    OS::get_singleton()->print("Start Transform checks.\n");

    bool success = true;

    if (!test_vector3()) {
        success = false;
    }

    if (!test_plane()) {
        success = false;
    }

    if (!test_aabb()) {
        success = false;
    }

    if (success) {
        OS::get_singleton()->print("Transform checks passed.\n");
    } else {
        OS::get_singleton()->print("Transform checks FAILED.\n");
    }

    return nullptr;
}

} // namespace TestTransform
