//
// Created by 44772 on 2024/2/13.
//

#pragma once

#include "core/object.h"
#include "core/interaction.h"

NAMESPACE_BEGIN

class Light : public Object {
public:
    virtual ~Light() = default;

    virtual Color3f SampleLi(const SurfaceInteraction &si, Vector3f &wi, Point2f &sample) const = 0;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::ELight;
    }
};



NAMESPACE_END