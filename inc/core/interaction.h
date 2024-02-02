//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/common.h"
#include "core/math.h"
NAMESPACE_BEGIN

class BxDF;

// default surface interaction
class Interaction {
public:
    Point3f p;// position
    Normal3f n;// normal
    Float t;// time
    Boolean front{};// Is face front
    BxDF* bxdf{nullptr};
};

NAMESPACE_END