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

    std::optional<Color3f>
    Sample(const SurfaceInteraction &si, const Vector3f wo, Vector3f &wi, Point2f sample) const override {
        wi = Reflect(si.wo, Vector3f{si.n});
        if (Dot(wi, si.n) > 0) {
            return mAlbedo;
        }
        return std::nullopt;
    }

private:
    Color3f mAlbedo;
};

REGISTER_CLASS(Reflection, "reflection")
NAMESPACE_END