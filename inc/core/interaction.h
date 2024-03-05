//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/common.h"
#include "core/math.h"
#include "util/frame.h"
#include "core/record.h"

NAMESPACE_BEGIN


// default surface interaction
//class IntersectionRecord {
//public:// members
//    Point3f p; // intersect world position
//    Normal3f n; // intersect world normal
//    Point2f uv; // intersect uv
//    Float t{Infinity};// intersect time
//    Frame shading;// Transform from world to shading coordinate
//
//    BxDF *bxdf{nullptr}; // intersect shape bxdf
//    Emitter *emitter{nullptr}; // intersect shape emitter
//public:
//    IntersectionRecord() = default;
//
//    IntersectionRecord(Float t, Point3f p, Normal3f n)
//            : t{t},
//              p{p},
//              n{Normalize(n)} {
//        shading = Frame(n);
//    }
//
//    void SetFlipNormal(Vector3f wi) {
//        n *= Dot(n,wi) ? 1 : -1;
//    }
//
//    // Generate ray from world coordinate
//    // Before using this function, you should transform the direction to world coordinate
//    Ray GenerateRay(Vector3f d) {
//        return Ray{p, d};
//    }
//
//    Color3f Le(Vector3f w);
//};

NAMESPACE_END