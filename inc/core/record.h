//
// Created by talentstream on 2024/3/3.
//

#pragma once

#include "core/common.h"
#include "core/math.h"
#include "util/frame.h"

NAMESPACE_BEGIN

class Sampler;

class BxDF;

class Emitter;

struct IntersectionRecord {
// members
    Point3f p; // intersect world position
    Normal3f n; // intersect world normal
    Point2f uv; // intersect uv
    Float t{Infinity};// intersect time
    Frame shading;// Transform from world to shading coordinate

    BxDF *bxdf{nullptr}; // intersect shape bxdf
    Emitter *emitter{nullptr}; // intersect shape emitter

// functions
    // Call to Initialize
    IntersectionRecord() = default;

    // When Intersect with shape, call this function to initialize
    IntersectionRecord(Float t, const Point3f &p, const Normal3f &n)
            : t{t},
              p{p},
              n{Normalize(n)} {
        shading = Frame(n);
    }

    // set normal direction
    void SetFlipNormal(const Vector3f &wi) {
        n *= Dot(n, wi) ? 1 : -1;
    }

    Vector3f ToLocal(const Vector3f &v) {
        return shading.ToLocal(v);
    }

    Vector3f ToWorld(const Vector3f &v) {
        return shading.ToWorld(v);
    }

    // Generate ray from world coordinate
    // Before using this function, you should transform the direction to world coordinate
    Ray GenerateRay(const Vector3f &d) {
        return Ray{p, d};
    }

    // Before use, should guarantee w is on world coordinate
    Color3f Le(const Vector3f& w);
};

struct BxDFRecord {
// members
    Vector3f wi;// incident direction in shading coordinate, should same hemisphere with normal
    Vector3f wo;// outgoing direction in shading coordinate
    Point2f uv;// for sample texture
    Point3f p;// intersect point

// functions
    // for call BxDF Sample Function
    // Before using, should transform wi to shading coordinate
    explicit BxDFRecord(const Vector3f &wi)
            : wi{wi} {}

    // before using, should transform wi, wo to shading coordinate
    // for query bxdf, if it knows wi, wo (in direct lighting?)
    explicit BxDFRecord(const Vector3f &wi, const Vector3f &wo)
            : wi{wi}, wo{wo} {}

};

struct ShapeRecord {
// members
    Point3f ref;// origin shape point
    Point3f p;// Sampled point on shape
    Normal3f n;// Sampled normal on shape in world coordinate
    Float pdf;// Probability of the sample

// functions
    // for call Shape Sample function
    explicit ShapeRecord(const Point3f &ref)
            : ref{ref} {}

    // for call Shape Pdf function
    explicit ShapeRecord(const Point3f &ref, const Point3f &p)
            : ref{ref}, p{p} {}
};

struct EmitterRecord {
// members
    Point3f ref; // origin emitter point
    Point3f p; // Sampled point on emitter
    Normal3f n; // Sampled normal on emitter in world coordinate
    Vector3f wi; // emitter to intersect point direction in world coordinate
    Float pdf; // Probability of the sample

// functions
    // for call Emitter Sample Function
    explicit EmitterRecord(const Point3f &ref)
            : ref{ref} {}

    // for call Emitter Pdf Function
    explicit EmitterRecord(const Point3f &ref, const Point3f &p, const Normal3f &n)
            : ref{ref}, p{p}, n{n} {
        wi = Normalize(p - ref);
    }
};

NAMESPACE_END