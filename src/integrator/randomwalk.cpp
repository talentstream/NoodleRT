//
// Created by talentstream on 2024/2/24.
//

#include "base/integrator.h"
#include "base/aggregate.h"
#include "base/camera.h"
#include "base/bxdf.h"
#include "base/light.h"
#include "base/sampler.h"
#include "core/record.h"
#include "util/sampling.h"

#include <print>

NAMESPACE_BEGIN

// simplified version of path tracing
// only support diffuse surface
class RandomWalkIntegrator : public ImageTileIntegrator {
public:
    explicit RandomWalkIntegrator(const PropertyList &propertyList)
    : ImageTileIntegrator(propertyList){
        mMaxDepth = propertyList.GetInteger("depth", 1);
        PRINT_DEBUG_INFO("Integrator", "randomwalk")
    }

    [[nodiscard]] Color3f Li(const Ray &ray) const override {
        return Trace(ray, 0);
    }

private:

    Color3f Trace(const Ray &ray, Integer depth) const {
        //Todo: find the problem
        IntersectionRecord si;
        if (!pAggregate->Intersect(ray, si)) {
            Color3f backgroundColor{0};
            for (const auto light: mLights) {
                backgroundColor += light->Le(ray);
            }
            return backgroundColor;
        }

        // shading
        Color3f Le{};
        auto bxdf = si.bxdf;
        Vector3f wo = -ray.d;
        Le = si.Le(wo);
        // Randomly Sample
        Point2f u = pSampler->Next2D();
        Vector3f wp = Warp::SampleUniformSphere(u);

        auto fcos = bxdf->F(si, wo, wp) * AbsDot(si.n, wp);
        if(depth == mMaxDepth) return Le + fcos;
        Ray scattered{si.p, wp};
        return Le + fcos * Trace(scattered, depth + 1) / (1 / (4 * Pi));
    }

    Integer mMaxDepth;
};

REGISTER_CLASS(RandomWalkIntegrator, "randomwalk")

NAMESPACE_END