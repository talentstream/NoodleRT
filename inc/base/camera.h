//
// Created by 44772 on 2024/1/27.
//

#pragma once

#include "core/object.h"

NAMESPACE_BEGIN

class Camera : public Object {
public:
    virtual ~Camera() = default;

    virtual Ray GenerateRay(Point2f uv) const = 0;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::ECamera;
    }
};

NAMESPACE_END