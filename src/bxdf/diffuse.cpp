//
// Created by 44772 on 2024/2/2.
//

#include "base/bxdf.h"
#include "base/texture.h"
#include <print>

NAMESPACE_BEGIN

// Ideal Diffuse
// Assume the same value for all incident and outgoing directions
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
            pAlbedo = dynamic_cast<Texture *>(ObjectFactory::CreateInstance("checker", {}, true));
        }
    }

    std::optional<Vector3f> ComputeScattering(const SurfaceInteraction &si, Vector3f wo) const override {
        Vector3f scatterDirection = Vector3f(si.n) + RandomInUnitVector();
        auto NearZero = [](Vector3f v) {
            return (Abs(v.x) < Epsilon) && (Abs(v.y) < Epsilon) && (Abs(v.z) < Epsilon);
        };
        if (NearZero(scatterDirection)) {
            scatterDirection = Vector3f(si.n);
        }
        return scatterDirection;
    }

    [[nodiscard]] Color3f Evaluate(const SurfaceInteraction &si, Vector3f wo) const override {
        return pAlbedo->Evaluate(si);
    }

private:
    Texture *pAlbedo{nullptr};
};

REGISTER_CLASS(Diffuse, "diffuse")

NAMESPACE_END