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

    std::optional<Color3f>
    Sample(const SurfaceInteraction &si, const Vector3f wo, Vector3f &wi, Point2f sample) const override {
        if (Dot(wo, si.n) < 0) {
            return std::nullopt;
        }
        wi = Vector3f{si.n} + RandomInUnitVector();
        return pAlbedo->Evaluate(si);
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

private:
    Texture *pAlbedo{nullptr};
};

REGISTER_CLASS(Diffuse, "diffuse")

NAMESPACE_END