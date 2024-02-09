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

        [[nodiscard]] Boolean
        ComputeScattering(const Ray &ray, const Interaction &i, Color3f &attenuation, Ray &wo) const override {
            // Todo: dielectric bug? 相反的结果
            attenuation = Color3f{1.0f, 1.0f, 1.0f};
            Float refractionRatio = i.front ? (1.0f / mRefractionIndex) : mRefractionIndex;
            Vector3f unitDirection = Normalize(ray.d);
            Float cosTheta = Min(Dot(-unitDirection, Vector3f(i.n)), 1.0f);
            Float sinTheta = Sqrt(1.0f - cosTheta * cosTheta);
            Boolean cannotRefract = refractionRatio * sinTheta > 1.0f;

            Vector3f direction;
            if (cannotRefract || Schlick(cosTheta, refractionRatio) > 0) {
                direction = Reflect(unitDirection, Vector3f(i.n));
            } else {
                direction = Refract(unitDirection, Vector3f(i.n), refractionRatio);
            }
            wo = Ray(i.p, direction);
            return true;
        }

    private:
        Float mRefractionIndex;
    };

    REGISTER_CLASS(Dielectric, "dielectric")

NAMESPACE_END