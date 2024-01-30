//
// Created by talentstream on 2024/1/20.
//

#pragma once

#include "core/common.h"
#include "math/vec.h"

NAMESPACE_BEGIN

class Ray {
public:
    Point3f o;
    Vector3f d;
    Float t{};

    Ray() = default;

    Ray(Point3f o, Vector3f d, Float t = 0.0f)
            : o{o}, d{d}, t{t} {}

    Point3f operator()(Float time) const { return o + d * time; }

};

NAMESPACE_END