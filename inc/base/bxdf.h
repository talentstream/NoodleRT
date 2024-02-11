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
    virtual std::optional<Vector3f> GenerateRay(SurfaceInteraction&, Vector3f wo) const {return std::nullopt;}

    virtual Color3f Evaluate(const SurfaceInteraction &, Vector3f wo) const {
        return {1,1,1};
    }

    virtual Boolean ComputeScattering(const Ray &, const SurfaceInteraction &, Color3f &, Ray &wo) const = 0;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::EBxDF;
    }
};

NAMESPACE_END
