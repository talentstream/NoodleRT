//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/object.h"

NAMESPACE_BEGIN

    class Material : public Object {
    public:
        virtual ~Material() = default;


        virtual Boolean ComputeScattering()
        [[nodiscard]] EClassType GetClassType() const override {
            return EClassType::EMaterial;
        }
    };

NAMESPACE_END
