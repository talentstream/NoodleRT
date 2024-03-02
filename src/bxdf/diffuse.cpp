//
// Created by 44772 on 2024/2/2.
//

#include "base/bxdf.h"
#include "base/texture.h"
#include "util/sampling.h"
#include <print>

NAMESPACE_BEGIN

// Ideal Diffuse, Lambertian
// Represents an ideally diffuse material with a user-specified amount of reflectance.
class Diffuse : public BxDF {
public:
    explicit Diffuse(const PropertyList &propertyList) {
        PRINT_DEBUG_INFO("BxDF", "diffuse")
    }

    Color3f
    F(const SurfaceInteraction &si, const Vector3f wo, const Vector3f wi) const override {
        if (Frame::CosTheta(wo) <= 0 ||
            Frame::CosTheta(wi) <= 0) {
            return {0.f};
        }
        return pAlbedo->Evaluate(si) * Frame::CosTheta(wo) * InvPi;
    }

    Float Pdf(const SurfaceInteraction &si, const Vector3f wo, const Vector3f wi) const override {
        if (Frame::CosTheta(wo) <= 0 ||
            Frame::CosTheta(wi) <= 0) {
            return 0.f;
        }

        return SquareToCosineHemispherePdf(wo);
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

    BxDFFlag Flag() const override {
        return EDiffuse;
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