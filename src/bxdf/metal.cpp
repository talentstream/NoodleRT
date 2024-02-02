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

        [[nodiscard]] Boolean
        ComputeScattering(const Ray &ray, const Interaction &i, Color3f &attenuation, Ray &wo) const override {
            Vector3f reflected = Reflect(Normalize(ray.d), Vector3f(i.n));
            wo = Ray(i.p, reflected + mRoughness * RandomInUnitSphere());
            attenuation = mAlbedo;
            return Dot(wo.d, Vector3f(i.n)) > 0;
        }

    private:
        Color3f mAlbedo;
        Float mRoughness{};
    };

    REGISTER_CLASS(Metal, "metal")

NAMESPACE_END