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

inline Boolean Refract(Vector3f wi, Vector3f &wo, Vector3f n, Float eta) {
    Float cosThetaI = Dot(n, wi);
    Float sin2ThetaI = Max(0, 1 - cosThetaI * cosThetaI);
    Float sin2ThetaT = eta * eta * sin2ThetaI;

    if (sin2ThetaT >= 1) {
        return false;
    }
    Float cosThetaT = sqrtf(1 - sin2ThetaT);
    wo = eta * -wi + (eta * cosThetaI - cosThetaT) * n;
    return true;
}

inline Float FrDielectric(Float cosThetaI, Float eta) {
    cosThetaI = Clamp(cosThetaI, -1.0f, 1.0f);
    if (cosThetaI < 0) {
        eta = 1 / eta;
        cosThetaI = -cosThetaI;
    }

    Float sin2ThetaI = 1 - Sqr(cosThetaI);
    Float sin2ThetaT = sin2ThetaI / Sqr(eta);
    if (sin2ThetaT >= 1) {
        return 1;
    }
    Float cosThetaT = Sqrt(1 - sin2ThetaT);

    Float rParallel = (eta * cosThetaI - cosThetaT) / (eta * cosThetaI + cosThetaT);
    Float rPerpendicular = (cosThetaI - eta * cosThetaT) / (cosThetaI + eta * cosThetaT);
    return (Sqr(rParallel) + Sqr(rPerpendicular)) / 2;
}

NAMESPACE_END
