//
// Created by talentstream on 2024/2/25.
//

#include "base/integrator.h"

NAMESPACE_BEGIN

class PathIntegrator : public ImageTileIntegrator {
public:
    explicit PathIntegrator(const PropertyList &propertyList) {
        mSpp = propertyList.GetInteger("spp", 1);
        mMaxDepth = propertyList.GetInteger("depth", 1);
        PRINT_DEBUG_INFO("Integrator", "path")
    }

    [[nodiscard]] Color3f Li(const Ray &ray) const override {
    }

private:
    Integer mMaxDepth;
};

NAMESPACE_END