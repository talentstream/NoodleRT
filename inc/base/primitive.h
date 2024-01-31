#pragma once

#include "core/object.h"
#include "core/interaction.h"
#include "base/shape.h"

NAMESPACE_BEGIN

    class Primitive : public Object {
    public:
        virtual ~Primitive() = default;

        virtual Boolean Intersect(const Ray &ray, Interaction& interaction) const = 0;

        [[nodiscard]] EClassType GetClassType() const override {
            return EClassType::EPrimitive;
        }

    private:
        Shape *pShape;
    };

NAMESPACE_END