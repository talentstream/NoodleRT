//
// Created by talentstream on 2024/2/29.
//

#include "core/record.h"
#include "base/light.h"

NAMESPACE_BEGIN

Color3f IntersectionRecord::Le(const Vector3f& w) {
    if(emitter) return emitter->L(*this, shading.ToLocal(w));
    return {0.f};
}

NAMESPACE_END