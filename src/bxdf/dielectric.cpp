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
        PRINT_DEBUG_INFO("BxDF", "dielectric")
    }

    std::optional<Color3f>
    Sample(const SurfaceInteraction &si, const Vector3f wo, Vector3f &wi, Point2f sample) const override {

    }

private:
    Float intIOR;
    Float extIOR;
};

NAMESPACE_END