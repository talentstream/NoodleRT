//
// Created by talentstream on 2024/2/24.
//

#include "base/integrator.h"
#include "base/aggregate.h"
#include "base/camera.h"
#include "base/bxdf.h"
#include "base/light.h"
#include "base/sampler.h"
#include "core/interaction.h"
#include "util/sampling.h"

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
        //Todo: find the problem
        SurfaceInteraction si;
        if(!pAggregate->Intersect(ray, si)) {
            Color3f backgroundColor{0};
            for(const auto light: mLights) {
                backgroundColor += light->Le(ray);
            }
            return backgroundColor;
        }

        // shading
        auto bxdf = si.bxdf;
        Vector3f wo = -ray.d, wi;
        auto Le = bxdf->Sample(si, wo, wi, pSampler->Next2D());
        if (!Le.has_value()) return {0};
        auto le = Le.value();

        // Randomly Sample
        Point2f u = pSampler->Next2D();
        Vector3f wp = SampleUniformSphere(u);

        auto fCos = Dot(wp, si.n);
        if(depth == mMaxDepth || fCos < 0) return le;

        Ray scattered{si.p,wp};
        return le + le * fCos * Trace(scattered, depth + 1) / (1 / (4 * Pi));
    }

    Integer mMaxDepth;
};

REGISTER_CLASS(RandomWalkIntegrator, "randomwalk")

NAMESPACE_END