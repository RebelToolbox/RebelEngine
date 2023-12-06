// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

#ifndef TRIANGULATE_H
#define TRIANGULATE_H

#include "core/math/vector2.h"

/*
http://www.flipcode.com/archives/Efficient_Polygon_Triangulation.shtml
*/

class Triangulate {
public:
    // triangulate a contour/polygon, places results in STL vector
    // as series of triangles.
    static bool triangulate(
        const Vector<Vector2>& contour,
        Vector<int>& result
    );

    // compute area of a contour/polygon
    static real_t get_area(const Vector<Vector2>& contour);

    // decide if point Px/Py is inside triangle defined by
    // (Ax,Ay) (Bx,By) (Cx,Cy)
    static bool is_inside_triangle(
        real_t Ax,
        real_t Ay,
        real_t Bx,
        real_t By,
        real_t Cx,
        real_t Cy,
        real_t Px,
        real_t Py,
        bool include_edges
    );

private:
    static bool snip(
        const Vector<Vector2>& p_contour,
        int u,
        int v,
        int w,
        int n,
        const Vector<int>& V,
        bool relaxed
    );
};

#endif
