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

        Boolean Intersect(const Ray &ray, Float tMax, Interaction &i) const override {
            Vector3f oc = ray.o - mCenter;
            Float a = Dot(ray.d, ray.d);
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
            i.t = root;
            i.p = ray(root);
            Normal3f outNormal{Normalize(i.p - mCenter)};
            i.front = Dot(ray.d, Vector3f(outNormal)) < 0;
            i.n = i.front ? outNormal : -outNormal;

            return true;
        }

    private:
        Float mRadius;
        Point3f mCenter;
    };

    REGISTER_CLASS(Sphere, "sphere")

NAMESPACE_END