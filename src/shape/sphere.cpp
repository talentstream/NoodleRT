//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "base/shape.h"
#include <print>

NAMESPACE_BEGIN

class Sphere : public Shape {
public:
    explicit Sphere(const PropertyList &propertyList) {
        mRadius = propertyList.GetFloat("radius", 1.0f);
        mCenter = propertyList.GetPoint("center", {});
        PRINT_DEBUG_INFO("Shape", "sphere")

    }

    Boolean
    Intersect(const Ray &ray, Float tMax, SurfaceInteraction &i) const override {
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

        // 设定Interaction Info

        Point3f hitP = ray(root);
        i = SurfaceInteraction(root, hitP, Normal3f{hitP - mCenter}, -ray.d);

        // uv
        auto theta = ACos(-i.n.y);
        auto phi = ATan2(-i.n.z, i.n.x) + Pi;
        i.u = phi / (2 * Pi);
        i.v = theta / Pi;
        return true;
    }

    Boolean
    IntersectP(const Ray &ray, Float tMax) const override {
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

    Bound3f
    BoundingBox() const override {
        Vector3f radiusVec{mRadius};
        return Bound3f{mCenter - radiusVec, mCenter + radiusVec};
    }

private:
    Float mRadius;
    Point3f mCenter;
};

REGISTER_CLASS(Sphere, "sphere")

NAMESPACE_END