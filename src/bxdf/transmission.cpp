//
// Created by 44772 on 2024/2/13.
//

#include "base/bxdf.h"
#include <print>

NAMESPACE_BEGIN

// Perfect Specular transmission
class Transmission : public BxDF {
public:
    explicit Transmission(const PropertyList &propertyList) {
        mIntIOR = propertyList.GetFloat("intIOR", 1.5f);
        mExtIOR = propertyList.GetFloat("extIOR", 1.0f);
        mAlbedo = propertyList.GetColor("albedo", {0.5f});
        PRINT_DEBUG_INFO("BxDF", "transmission")
    }

    Color3f F(const SurfaceInteraction &si, const Vector3f wo, const Vector3f wi) const override {
        return mAlbedo;
    }

    Float Pdf(const SurfaceInteraction &si, const Vector3f wo, const Vector3f wi) const override{
        return 1.f;
    }

    std::optional<Color3f>
    SampleF(const SurfaceInteraction &si, const Vector3f wo, Vector3f &wi, Point2f sample) const override {
        // Todo: test this
        Float eta = mExtIOR / mIntIOR;
        eta = Frame::CosTheta(wo) > 0 ? eta : 1 / eta;
        if (RefractLocal(wo, wi, eta)) {
            return mAlbedo;
        }
        return std::nullopt;
    }

    BxDFFlag Flag() const override {
        return ETransmission;
    }

private:
    Float mIntIOR;
    Float mExtIOR;
    Color3f mAlbedo;
};

REGISTER_CLASS(Transmission, "transmission")

NAMESPACE_END