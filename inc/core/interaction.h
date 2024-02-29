//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/common.h"
#include "core/math.h"
#include "util/frame.h"

NAMESPACE_BEGIN

class BxDF;
class Light;
class Frame;

// default surface interaction
class SurfaceInteraction {
public:
    SurfaceInteraction() = default;

    SurfaceInteraction(Float t, Point3f p, Normal3f n, Vector3f wo)
            : t{t},
              p{p},
              n{Normalize(n)},
              wo{Normalize(wo)} {
        SetFrontFace();
        shading = Frame(n);
    }

    void SetFrontFace(){
        front = Dot(n, wo) > 0;
        n *= front ? 1 : -1;
    }

    Ray GenerateRay(Vector3f d) {
       return Ray{p,shading.ToWorld(d)};
    }

    Color3f Le(Vector3f w);


    Point3f p;// position
    Float t{Infinity};// intersect time
    Vector3f wo;// out direction
    Normal3f n;// normal
    Boolean front{};// Is normal face front

    Frame shading;

    // texture uv
    Float u{};
    Float v{};

    // material
    BxDF *bxdf{nullptr};

    // emissive light
    Light *areaLight{nullptr};
};

NAMESPACE_END