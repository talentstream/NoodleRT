//
// Created by talentstream on 2024/3/3.
//

#include "base/integrator.h"
#include <print>

NAMESPACE_BEGIN

class DirectLightIntegrator : public ImageTileIntegrator {
public:
    explicit DirectLightIntegrator(const PropertyList &propertyList) {
        mSpp = propertyList.GetInteger("spp", 1);
        mMaxDepth = propertyList.GetInteger("depth", 1);
        PRINT_DEBUG_INFO("Integrator", "directlight")
    }

    [[nodiscard]] Color3f Li(const Ray &ray) const override {
        return {0.f};
    }

private:
    Integer mMaxDepth;
};

REGISTER_CLASS(DirectLightIntegrator, "directlight")

NAMESPACE_END