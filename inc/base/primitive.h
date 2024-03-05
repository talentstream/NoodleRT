#pragma once

#include "core/object.h"
#include "core/record.h"

NAMESPACE_BEGIN

class Primitive : public Object {
public:
    virtual ~Primitive() = default;

    virtual Boolean
    Intersect(const Ray &ray, IntersectionRecord &interaction) const = 0;

    virtual Boolean
    IntersectP(const Ray &ray) const = 0;

    virtual Bound3f
    BoundingBox() const = 0;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::EPrimitive;
    }
};

NAMESPACE_END