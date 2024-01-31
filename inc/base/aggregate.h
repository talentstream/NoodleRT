//
// Created by 44772 on 2024/1/31.
//

#pragma once

#include "core/object.h"
#include "core/interaction.h"

NAMESPACE_BEGIN

    class Aggregate : public Object {
    public:
        virtual ~Aggregate() = default;

        virtual Boolean Intersect(const Ray &ray, Interaction &interaction) const = 0;

        [[nodiscard]] EClassType GetClassType() const override {
            return EClassType::EAggregate;
        }
    };

NAMESPACE_END