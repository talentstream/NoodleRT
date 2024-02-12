//
// Created by talentstream on 2024/1/20.
//

#pragma once

#include "core/common.h"
#include "math/vec.h"

NAMESPACE_BEGIN

// default input w & output r is outside the surface
// return r
inline Vector3f Reflect(Vector3f w, Vector3f n) {
    w = -w;
    // w is into the surface
    return w - 2 * Dot(w, n) * n;
}

inline Vector3f Refract(Vector3f wi, Vector3f n, Float etai_over_etat) {
    Float cos_theta = std::min(Dot(-wi, n), 1.0f);
    Vector3f r_out_parallel = etai_over_etat * (wi + cos_theta * n);
    Vector3f r_out_perp = -sqrtf(fabsf(1.0f - LengthSquared(r_out_parallel))) * n;
    return r_out_parallel + r_out_perp;
}

inline Float Schlick(Float cosine, Float ref_idx) {
    Float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * powf((1 - cosine), 5);
}

NAMESPACE_END
