//
// Created by talentstream on 2024/2/24.
//

#include "base/integrator.h"
#include "base/aggregate.h"
#include "base/camera.h"
#include "base/bxdf.h"
#include "base/light.h"
#include "core/interaction.h"

#include <print>

NAMESPACE_BEGIN

class RandomWalkIntegrator : public ImageTileIntegrator {
public:
    explicit RandomWalkIntegrator(const PropertyList &propertyList) {
        mMaxDepth = propertyList.GetInteger("depth", 1);
        mSpp = propertyList.GetInteger("spp", 1);
        PRINT_DEBUG_INFO("Integrator", "randomwalk")
    }

    [[nodiscard]] Color3f Li(const Ray &ray) const override {
        return Trace(ray, 0);
    }

private:

    Color3f Trace(const Ray &ray, Integer depth) const {

    }

    Integer mMaxDepth;
};

NAMESPACE_END