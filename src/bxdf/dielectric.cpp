//
// Created by 44772 on 2024/2/2.
//

#include "base/bxdf.h"
#include <print>

NAMESPACE_BEGIN

class Dielectric : public BxDF {
public:
    explicit Dielectric(const PropertyList &propertyList) {
        mRefractionIndex = propertyList.GetFloat("refractionIndex", 1.0f);
        PRINT_DEBUG_INFO("BxDF", "dielectric")

    }

    std::optional<Vector3f> ComputeScattering(const SurfaceInteraction &si, Vector3f wo) const override {
        Float refractionRatio = si.front ? (1.0f / mRefractionIndex) : mRefractionIndex;
        Vector3f unitDirection = Normalize(wo);
        Float cosTheta = Min(Dot(-unitDirection, Vector3f(si.n)), 1.0f);
        Float sinTheta = Sqrt(1.0f - cosTheta * cosTheta);
        Boolean cannotRefract = refractionRatio * sinTheta > 1.0f;

        if(cannotRefract || Schlick(cosTheta, refractionRatio) > RandomFloat()){
            return Reflect(unitDirection, Vector3f(si.n));
        }else{
            return Refract(unitDirection, Vector3f(si.n), refractionRatio);
        }
    }

    Color3f Evaluate(const SurfaceInteraction &si, Vector3f wo) const override {
        return {1.0f, 1.0f, 1.0f};
    }

private:
    Float mRefractionIndex;
};

REGISTER_CLASS(Dielectric, "dielectric")

NAMESPACE_END