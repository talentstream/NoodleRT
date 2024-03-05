//
// Created by 44772 on 2024/2/5.
//

#pragma once

#include "core/object.h"

NAMESPACE_BEGIN

class Texture : public Object {
public:
    virtual ~Texture() = default;

    virtual Color3f Evaluate(const Point2f &) const = 0;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::ETexture;
    }
};

NAMESPACE_END