//
// Created by talentstream on 2024/1/20.
//

#pragma once

#include "core/common.h"
#include "math/vec.h"
#include "math/constant.h"

NAMESPACE_BEGIN

// default w & n is same side of surface
inline Vector3f Reflect(Vector3f w, Vector3f n) {
    return -w + 2 * Dot(w, n) * n;
}

inline Vector3f Refract(Vector3f wi, Vector3f n, Float ratio) {
    Float costTheta = Min(Dot(-wi, n), 1.0);
    Vector3f rOutParallel = ratio * (wi + costTheta * n);
    Vector3f rOutPerp = -Sqrt(Abs(1.0f - LengthSquared(rOutParallel))) * n;
    return rOutParallel + rOutPerp;

}

inline Float Schlick(Float cosine, Float refIdx) {
    Float r0 = (1 - refIdx) / (1 + refIdx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * powf((1 - cosine), 5);
}

NAMESPACE_END
