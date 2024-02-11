//
// Created by 44772 on 2024/2/9.
//

#include "base/bxdf.h"
#include "base/texture.h"
#include <print>

NAMESPACE_BEGIN

// Smooth Diffuse
class Lambert : public BxDF {
public:
    explicit Lambert(const PropertyList &propertyList) {
        PRINT_DEBUG_INFO("BxDF", "lambert")
    }

    std::optional<Vector3f> GenerateRay(SurfaceInteraction& si, Vector3f wo) const override{
        return std::nullopt;
    }

    Color3f Evaluate(const SurfaceInteraction &si, Vector3f wo) const override{
        return pAlbedo->Evaluate(si);
    }

    [[nodiscard]]
    Boolean
    ComputeScattering(const Ray &ray, const SurfaceInteraction &i, Color3f &attenuation, Ray &wo) const override {
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