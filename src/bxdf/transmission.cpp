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
        mAlbedo = propertyList.GetColor("albedo", {1});
        PRINT_DEBUG_INFO("BxDF", "transmission")
    }

    std::optional<Color3f>
    Sample(const SurfaceInteraction &si, const Vector3f wo, Vector3f &wi, Point2f sample) const override {
        Float ratio = mIntIOR / mExtIOR;
        Float cosTheta = AbsDot(si.wo, si.n);
        Float sinTheta = Sqrt(1 - cosTheta * cosTheta);
        if (ratio * sinTheta > 1) {
            return std::nullopt;
        }
        wi = Refract(si.wo, Vector3f{si.n}, ratio);
        return mAlbedo;
    }

private:
    Float mIntIOR;
    Float mExtIOR;
    Color3f mAlbedo;
};

REGISTER_CLASS(Transmission, "transmission")

NAMESPACE_END