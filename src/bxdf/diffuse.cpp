//
// Created by 44772 on 2024/2/2.
//

#include "base/bxdf.h"

NAMESPACE_BEGIN

    class Diffuse : public BxDF {
    public:
        explicit Diffuse(const PropertyList &propertyList) {
            mAlbedo = propertyList.GetColor("albedo", {1, 1, 1});
        }

        [[nodiscard]] Boolean
        ComputeScattering(const Ray &ray, const Interaction &i, Color3f &attenuation, Ray &wo) const override {
            Vector3f scatterDirection = Vector3f(i.n) + RandomInUnitVector();
//            if (NearZero(scatterDirection)) {
//                scatterDirection = Vector3f(i.n);
//            }
            wo = Ray(i.p, scatterDirection);
            attenuation = mAlbedo;
            return true;
        }

    private:
        Color3f mAlbedo;
    };

    REGISTER_CLASS(Diffuse, "diffuse")

NAMESPACE_END