//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "base/shape.h"
#include "util/sampling.h"
#include <print>

NAMESPACE_BEGIN

class Sphere : public Shape {
public:
    explicit Sphere(const PropertyList &propertyList) {
        mRadius = propertyList.GetFloat("radius", 1.0f);
        mCenter = propertyList.GetPoint("center", {});
        mBbox = Bound3f{mCenter - Vector3f{mRadius}, mCenter + Vector3f{mRadius}};
        PRINT_DEBUG_INFO("shape", "sphere")
    }

    Bool
    Intersect(UInt32 idx, const Ray &ray, IntersectionRecord &iRec) const override {

        Vector3f oc = ray.o - mCenter;

        Float a = LengthSquared(ray.d);
        Float halfB = Dot(oc, ray.d);
        Float c = LengthSquared(oc) - mRadius * mRadius;
        Float discriminant = halfB * halfB - a * c;
        if (discriminant < 0) {
            return false;
        }

        Float sqrtD = Sqrt(discriminant);
        Float root = (-halfB - sqrtD) / a;
        // 判断是否相交
        if (root < 0.001f || root > iRec.t) {
            root = (-halfB + sqrtD) / a;
            if (root < 0.001f || root > iRec.t) {
                return false;
            }
        }

        // 设定Interaction Info
        Point3f hitP = ray(root);
        Normal3f hitN = Normal3f{hitP - mCenter};

        iRec = IntersectionRecord(root, hitP, hitN);
        iRec.SetFlipNormal(ray.d);

        iRec.bxdf = pBxDF;
        iRec.emitter = pEmitter;

        // uv
        auto theta = ACos(-iRec.n.y);
        auto phi = ATan2(-iRec.n.z, iRec.n.x) + Pi;
        iRec.uv = Point2f{phi / (2 * Pi), theta / Pi};
        return true;
    }


    Bool
    IntersectP(UInt32 idx, const Ray &ray, Float tMax) const override {
        Vector3f oc = ray.o - mCenter;
        Float a = LengthSquared(ray.d);
        Float halfB = Dot(oc, ray.d);
        Float c = LengthSquared(oc) - mRadius * mRadius;
        Float discriminant = halfB * halfB - a * c;
        if (discriminant < 0) {
            return false;
        }

        Float sqrtD = Sqrt(discriminant);
        Float root = (-halfB - sqrtD) / a;
        // 判断是否相交
        if (root < 0.001 || root > tMax) {
            root = (-halfB + sqrtD) / a;
            if (root < 0.001 || root > tMax) {
                return false;
            }
        }
        return true;
    }

    void
    Sample(ShapeRecord &sRec, const Point2f &sample) const override {
        Vector3f dir = Warp::UniformSampleSphere(sample);
        sRec.p = mCenter + mRadius * dir;
        sRec.n = Normal3f{Normalize(dir)};
        sRec.pdf = 1.f / Area();
    }

    Float
    Pdf(const ShapeRecord &sRec) const override {
        return 0.f;
    }

    Float
    Area() const override {
        return 4 * Pi * mRadius * mRadius;
    }

    UInt32
    GetPrimitiveCount() const override {
        return 1;
    }

    Bound3f
    GetBoundingBox(UInt32 idx) const override {
        return mBbox;
    }

    Point3f
    GetCentroid(UInt32 idx) const override {
        return mCenter;
    }

private:
    Float mRadius;
    Point3f mCenter;
};

REGISTER_CLASS(Sphere, "sphere")

NAMESPACE_END