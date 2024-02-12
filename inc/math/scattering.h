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

inline Boolean Refract(Vector3f wi, Vector3f &wo, Vector3f n, Float ratio) {
    Float cosTheta = Dot(n, wi);
    Float sin2ThetaI = Max(0, 1 - cosTheta * cosTheta);
    Float sin2ThetaT = ratio * ratio * sin2ThetaI;

    if(sin2ThetaT >= 1) {
        return false;
    }
    Float cosThetaT = sqrtf(1 - sin2ThetaT);
    wo = ratio * -wi + (ratio * cosTheta - cosThetaT) * n;
    return true;
}

inline Float Schlick(Float cosine, Float refIdx) {
    Float r0 = (1 - refIdx) / (1 + refIdx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * powf((1 - cosine), 5);
}

NAMESPACE_END
