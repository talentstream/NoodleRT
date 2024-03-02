//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/object.h"
#include "core/interaction.h"

NAMESPACE_BEGIN

class Shape : public Object {
public:
    virtual ~Shape() = default;

    virtual Boolean
    Intersect(const Ray &ray, Float tMax, SurfaceInteraction &) const = 0;

    virtual Boolean
    IntersectP(const Ray &ray, Float tMax) const = 0;

    virtual Bound3f
    BoundingBox() const = 0;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::EShape;
    }
};

NAMESPACE_END