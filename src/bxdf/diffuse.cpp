//
// Created by 44772 on 2024/2/2.
//

#include "base/bxdf.h"
#include <print>

NAMESPACE_BEGIN

    class Diffuse : public BxDF {
    public:
        explicit Diffuse(const PropertyList &propertyList) {
            mAlbedo = propertyList.GetColor("albedo", {1.0f, 1.0f, 1.0f});
            PRINT_DEBUG_INFO("BxDF", "diffuse")

        }

        [[nodiscard]] Boolean
        ComputeScattering(const Ray &ray, const Interaction &i, Color3f &attenuation, Ray &wo) const override {

            Vector3f scatterDirection = Vector3f(i.n) + RandomInUnitVector();
            auto NearZero = [](const Vector3f &v) {
                const Float s = Epsilon;
                return (Abs(v.x) < s) && (Abs(v.y) < s) && (Abs(v.z) < s);
            };
            if (NearZero(scatterDirection)) {
                scatterDirection = Vector3f(i.n);
            }
            attenuation = mAlbedo;
            wo = Ray(i.p, scatterDirection);

            return true;
        }

    private:
        Color3f mAlbedo;
    };

    REGISTER_CLASS(Diffuse, "diffuse")

NAMESPACE_END