//
// Created by 44772 on 2024/2/12.
//

#include "base/bxdf.h"
#include <print>

NAMESPACE_BEGIN

// Perfect Specular reflection
class Reflection : public BxDF {
public:
    explicit Reflection(const PropertyList &propertyList) {
        mAlbedo = propertyList.GetColor("albedo", {0.5});
        PRINT_DEBUG_INFO("BxDF", "specular")
    }

    Color3f F(const SurfaceInteraction &si, const Vector3f wo, const Vector3f wi) override {
        return mAlbedo;
    }

    Float Pdf(const SurfaceInteraction &si, const Vector3f wo, const Vector3f wi) const {
        return 1.f;
    }

    std::optional<Color3f>
    SampleF(const SurfaceInteraction &si, const Vector3f wo, Vector3f &wi, Point2f sample) const override {
        wi = ReflectLocal(wo);

        if (Frame::CosTheta(wo) < 0 ||
            Frame::CosTheta(wi) < 0) {
            return std::nullopt;
        }

        return mAlbedo;
    }

private:
    Color3f mAlbedo;
};

REGISTER_CLASS(Reflection, "reflection")
NAMESPACE_END