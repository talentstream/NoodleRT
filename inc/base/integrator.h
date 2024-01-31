//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/object.h"

NAMESPACE_BEGIN

    class Integrator : public Object {
    public:
        virtual ~Integrator() = default;

        virtual Color3f Li(const Ray &ray, Integer depth = 0) const = 0;

        [[nodiscard]] EClassType GetClassType() const override {
            return EClassType::EIntegrator;
        }
    };

NAMESPACE_END