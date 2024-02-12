//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/common.h"
#include "core/math.h"

NAMESPACE_BEGIN

class BxDF;

// default surface interaction
class SurfaceInteraction {
public:
    SurfaceInteraction() = default;

    SurfaceInteraction(Float t, Point3f p, Normal3f n, Vector3f wo)
            : t{t},
              p{p},
              n{Normalize(n)},
              wo{Normalize(wo)} {}


    Float t{Infinity};// time
    Point3f p;// position
    Vector3f wo;// out direction
    Normal3f n;// normal
    Boolean front{};// Is normal face front

    // texture uv
    Float u{};
    Float v{};

    // material
    BxDF *bxdf{nullptr};
};

NAMESPACE_END