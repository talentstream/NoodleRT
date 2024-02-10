//
// Created by 44772 on 2024/2/9.
//

#include "base/bxdf.h"
#include "base/texture.h"
#include <print>

NAMESPACE_BEGIN

class Lambert : public BxDF {
public:
    explicit Lambert(const PropertyList &propertyList) {

        PRINT_DEBUG_INFO("BxDF", "lambert")
    }

    Boolean
    ComputeScattering(const Ray &ray, const Interaction &i, Color3f &attenuation, Ray &wo) const override {
        attenuation = pAlbedo->Evaluate(i);
        return false;
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
        }

    }


private:
    Texture *pAlbedo{nullptr};
};

REGISTER_CLASS(Lambert, "lambert")

NAMESPACE_END