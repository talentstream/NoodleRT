//
// Created by 44772 on 2024/2/2.
//

#include "base/bxdf.h"
#include "base/texture.h"
#include "util/sampling.h"
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
    F(const SurfaceInteraction &si, const Vector3f wo, const Vector3f wi) override {
        if (Frame::CosTheta(wo) <= 0 ||
            Frame::CosTheta(wi) <= 0) {
            return std::nullopt;
        }
        return pAlbedo->Evaluate(si) * InvPi;
    }

    std::optional<Color3f>
    SampleF(const SurfaceInteraction &si, const Vector3f wo, Vector3f &wi, Point2f sample2) const override {
        if (Frame::CosTheta(wo) < 0) return std::nullopt;

        wi = SquareToCosineHemisphere(sample2);
        // (brdf / pdf) * cos = [(albedo / pi) / (cos / pi)] * cos
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