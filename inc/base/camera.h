//
// Created by 44772 on 2024/1/27.
//

#pragma once

#include "object.h"

NAMESPACE_BEGIN

class Camera : public Object {
public:
    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::ECamera;
    }
};

NAMESPACE_END