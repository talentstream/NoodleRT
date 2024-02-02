//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/object.h"

NAMESPACE_BEGIN

    class BxDF : public Object {
    public:
        virtual ~BxDF() = default;


        virtual Boolean ComputeScattering() const = 0;

        [[nodiscard]] EClassType GetClassType() const override {
            return EClassType::EBxDF;
        }
    };

NAMESPACE_END
