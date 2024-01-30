//
// Created by talentstream on 2024/1/20.
//

#pragma once

#include "core/common.h"
#include "math/vec.h"
#include "math/color.h"
#include <random>

NAMESPACE_BEGIN

inline Integer RandomInt() {
    static std::uniform_int_distribution<Integer> distribution;
    static std::mt19937 generator;
    return distribution(generator);
}

inline Integer RandomInt(Integer min, Integer max) {
    return min + RandomInt() % (max - min + 1);
}

inline Float RandomFloat() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline Float RandomFloat(Float min, Float max) {
    return min + (max - min) * RandomFloat();
}

inline Vector3f RandomVector3f() {
    return Vector3f{RandomFloat(), RandomFloat(), RandomFloat()};
}

inline Vector3f RandomVector3f(Float min, Float max) {
    return Vector3f{RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max)};
}

inline Point3f RandomPoint3f() {
    return Point3f{RandomFloat(), RandomFloat(), RandomFloat()};
}

inline Point3f RandomPoint3f(Float min, Float max) {
    return Point3f{RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max)};
}

inline Color3f RandomColor3f() {
    return Color3f{RandomFloat(), RandomFloat(), RandomFloat()};
}

inline Color3f RandomColor3f(Float min, Float max) {
    return Color3f{RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max)};
}

inline Vector3f RandomInUnitSphere() {
    while (true) {
        auto p = RandomVector3f(-1, 1);
        if (LengthSquared(p) >= 1)
            continue;
        return p;
    }
}

inline Vector3f RandomInUnitVector() {
    return Normalize(RandomInUnitSphere());
}

inline Vector3f RandomInHemisphere(const Vector3f &normal) {
    auto in_unit_sphere = RandomInUnitSphere();
    if (Dot(in_unit_sphere, normal) > 0) {
        return in_unit_sphere;
    } else {
        return -in_unit_sphere;
    }
}


NAMESPACE_END
