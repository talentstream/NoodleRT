//
// Created by talentstream on 2024/3/3.
//

#pragma once

#include "core/common.h"
#include "core/math.h"

NAMESPACE_BEGIN

class ShapeSampleRecord{
public:
    Point3f ref;// origin reference point
    Point3f p;// Sampled point
    Normal3f n;// Sampled normal
    Float pdf;// Probability of the sample
};

NAMESPACE_END