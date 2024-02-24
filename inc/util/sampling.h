//
// Created by talentstream on 2024/2/24.
//

#pragma once

#include "core/math.h"

NAMESPACE_BEGIN

inline Vector3f SampleUniformSphere(Point2f u) {
    Float z = 1 - 2 * u[0];
    Float r = Sqrt(1 - Sqr(z));
    Float phi = 2 * Pi * u[1];
    return {r * Cos(phi), r * Sin(phi), z};
}

NAMESPACE_END
