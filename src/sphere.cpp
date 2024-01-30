//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "base/shape.h"
#include <print>
NAMESPACE_BEGIN

    class Sphere : public Shape {
    public:
        explicit Sphere(const PropertyList &propertyList)
        {
            std::print("->Sphere\n");
            mRadius = propertyList.GetFloat("radius", 1.0f);
            mCenter = propertyList.GetPoint("center",{});
        }

        Boolean Intersect(const Ray &ray, Float tMax, Interaction &i) const override {
            return true;
        }

    private:
        Float mRadius;
        Point3f mCenter;
    };

    REGISTER_CLASS(Sphere, "sphere")

NAMESPACE_END