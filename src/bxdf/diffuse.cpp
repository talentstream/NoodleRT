//
// Created by 44772 on 2024/2/2.
//

#include "base/bxdf.h"
#include "base/texture.h"
#include <print>

NAMESPACE_BEGIN

    class Diffuse : public BxDF {
    public:
        explicit Diffuse(const PropertyList &propertyList) {
            PRINT_DEBUG_INFO("BxDF", "diffuse")
        }

        void AddChild(Object *object) override {
            switch (object->GetClassType()) {
                case EClassType::ETexture:
                    pAlbedo = dynamic_cast<Texture *>(object);
                    break;
            }
        }

        void Initialize() override {
            if (pAlbedo == nullptr) {
                pAlbedo = dynamic_cast<Texture *>(ObjectFactory::CreateInstance("checker", {}));
                pAlbedo->Initialize();
            }
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
            attenuation = pAlbedo->Evaluate(i);
            wo = Ray(i.p, scatterDirection);

            return true;
        }

    private:
        Texture *pAlbedo{nullptr};
    };

    REGISTER_CLASS(Diffuse, "diffuse")

NAMESPACE_END