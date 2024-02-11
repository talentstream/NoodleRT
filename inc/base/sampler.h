//
// Created by 44772 on 2024/2/8.
//

#pragma once

#include "core/object.h"

NAMESPACE_BEGIN

class Sampler : public Object {
public:
    virtual ~Sampler() = default;

    virtual Float Next1D() = 0;

    virtual Point2f Next2D() = 0;

    virtual EClassType GetClassType() const override {
        return EClassType::ESampler;
    }
};

NAMESPACE_END


