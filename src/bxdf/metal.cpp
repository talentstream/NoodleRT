//
// Created by 44772 on 2024/2/2.
//

#include "base/bxdf.h"
#include <print>

NAMESPACE_BEGIN

class Metal : public BxDF {
public:
    explicit Metal(const PropertyList &propertyList) {
        mAlbedo = propertyList.GetColor("albedo", {});
        mRoughness = propertyList.GetFloat("roughness", 0.0f);
        PRINT_DEBUG_INFO("BxDF", "metal")

    }

    std::optional<Vector3f> ComputeScattering(const SurfaceInteraction &si, Vector3f wo) const override {
        Vector3f reflected = Reflect(Normalize(wo), Vector3f(si.n)) + mRoughness * RandomInUnitSphere();
        if(Dot(reflected, Vector3f(si.n)) > 0){
            return reflected;
        }
        return std::nullopt;
    }

    Color3f Evaluate(const SurfaceInteraction &si, Vector3f wo) const override {

        return mAlbedo;
    }

private:
    Color3f mAlbedo;
    Float mRoughness{};
};

REGISTER_CLASS(Metal, "metal")

NAMESPACE_END