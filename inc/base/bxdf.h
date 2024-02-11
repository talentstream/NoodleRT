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

    // compute scatter
    virtual std::optional<Vector3f> ComputeScattering(const SurfaceInteraction &si, Vector3f wo) const = 0;

    virtual Color3f Evaluate(const SurfaceInteraction &si, Vector3f wo) const = 0;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::EBxDF;
    }
};

NAMESPACE_END
