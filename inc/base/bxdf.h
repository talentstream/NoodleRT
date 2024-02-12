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
    virtual std::optional<Vector3f> ComputeScattering(const SurfaceInteraction &si, Vector3f wo) const { return {}; }

    virtual Color3f Evaluate(const SurfaceInteraction &si, Vector3f wo) const { return {}; }

    virtual std::optional<Color3f>
    Sample(const SurfaceInteraction &si, const Vector3f wo, Vector3f &wi, Point2f sample) const { return {}; };

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::EBxDF;
    }
};

NAMESPACE_END
