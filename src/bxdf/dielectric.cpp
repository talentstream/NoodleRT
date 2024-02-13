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

    std::optional<Color3f>
    Sample(const SurfaceInteraction &si, const Vector3f wo, Vector3f &wi, Point2f sample) const override {
        Float cosThetaI = Min(Dot(si.wo, si.n),1);
        Float ratio = intIOR / extIOR;
        if(FrDielectric(cosThetaI,ratio) > RandomFloat())
        {
            if(Refract(si.wo,wi,Vector3f{si.n},ratio)) {
                return Color3f{1};
            }
            else {
                return std::nullopt;
            }
        }
        wi = Reflect(si.wo, Vector3f{si.n});
        return Color3f{1};
    }

private:
    Float intIOR;
    Float extIOR;
    Float eta;
};

REGISTER_CLASS(Dielectric, "dielectric")

NAMESPACE_END