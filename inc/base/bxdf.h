//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/object.h"
#include "core/interaction.h"

NAMESPACE_BEGIN

class BxDF : public Object {
public:
    virtual ~BxDF() = default;

    // eval bxdf model
    virtual std::optional<Color3f>
    eval(const SurfaceInteraction &si, const Vector3f wo, const Vector3f wi) { return std::nullopt; }

    virtual std::optional<Color3f>
    Sample(const SurfaceInteraction &si, const Vector3f wo, Vector3f &wi, Point2f sample) const = 0;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::EBxDF;
    }
};

NAMESPACE_END
