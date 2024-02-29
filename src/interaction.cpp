//
// Created by talentstream on 2024/2/29.
//

#include "core/interaction.h"
#include "base/light.h"

NAMESPACE_BEGIN

Color3f SurfaceInteraction::Le(Vector3f w) {
    if(areaLight) return areaLight->L(*this, w);
    return {0.f};
}

NAMESPACE_END