//
// Created by 44772 on 2024/2/12.
//

#include "base/bxdf.h"
#include <print>

NAMESPACE_BEGIN

class Dielectric : public BxDF {
public:
    explicit Dielectric(const PropertyList &propertyList) {
        intIOR = propertyList.GetFloat("intIOR", 1.5f);
        extIOR = propertyList.GetFloat("extIOR", 1.0f);
        eta = propertyList.GetFloat("eta", 0.5f);
        PRINT_DEBUG_INFO("BxDF", "dielectric")
    }

    Color3f F(const SurfaceInteraction &si, const Vector3f wo, const Vector3f wi) override {
        return {0.f};
    }

    Float Pdf(const SurfaceInteraction &si, const Vector3f wo, const Vector3f wi) const {
        return 0.0f;
    }

    std::optional<Color3f>
    SampleF(const SurfaceInteraction &si, const Vector3f wo, Vector3f &wi, Point2f sample) const override {
        Float cosThetaI = Min(Frame::CosTheta(wo), 1);
        Float ratio = intIOR / extIOR;
        if (FrDielectric(cosThetaI, ratio) > eta) {
            if (RefractLocal(wo, wi, ratio)) {
                return Color3f{.5};
            }
        }
        wi = ReflectLocal(wo);
        return Color3f{.5};
    }

private:
    Float intIOR;
    Float extIOR;
    Float eta;
};

REGISTER_CLASS(Dielectric, "dielectric")

NAMESPACE_END