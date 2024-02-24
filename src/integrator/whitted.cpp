//
// Created by 44772 on 2024/1/31.
//

#include "base/integrator.h"
#include "base/aggregate.h"
#include "base/camera.h"
#include "base/bxdf.h"
#include "base/light.h"
#include "core/interaction.h"

#include <print>

NAMESPACE_BEGIN

class WhittedIntegrator : public ImageTileIntegrator {
public:
    explicit WhittedIntegrator(const PropertyList &propertyList) {
        mMaxDepth = propertyList.GetInteger("depth", 1);
        mSpp = propertyList.GetInteger("spp", 1);
        PRINT_DEBUG_INFO("Integrator", "whitted")
    }

    Color3f Li(const Ray &ray) const override {
        return Trace(ray, 0);
    }

private:
    Color3f Trace(const Ray &ray, Integer depth) const {
        // find nearest intersection
        SurfaceInteraction si;

        {
//            Vector3f unitDir = Normalize(ray.d);
//            Float t = 0.5f * (unitDir.y + 1.0f);
//            backgroundColor = (1.0f - t) * Color3f{1.0f, 1.0f, 1.0f} + t * Color3f{0.5f, 0.7f, 1.0f};
//            backgroundColor = {0, 0, 0};
        }
        Color3f backgroundColor{0};
        if (!pAggregate->Intersect(ray, si)) {

            for(const auto light: mLights) {
                backgroundColor += light->Le(ray);
            }
            return backgroundColor;
        }

        // shading
        auto bxdf = si.bxdf;
        Vector3f wo = -ray.d, wi;

        auto Le = bxdf->Sample(si, wo, wi, Point2f{});
        if (!Le.has_value()) return {1};
        auto le = Le.value();

        // calculate direct illumination
        Color3f emitted{};
        for (const auto light: mLights) {
            Point2f lightSample{0.5f, 0.5f};
            Vector3f w;// direction to light
            Color3f li = light->SampleLi(si, w, lightSample);
            Ray lightRay{si.p,w,si.t};
            SurfaceInteraction lightSi;
            if(!pAggregate->Intersect(lightRay, lightSi)) {
                emitted = li * le * Abs(Dot(w,si.n));
            }
        }
        if (depth == mMaxDepth) return emitted + le;

        // calculate indirect illumination

        Ray scattered{si.p, wi};
        return emitted + le * Trace(scattered, depth + 1);

    }

private:
    Integer mMaxDepth;
};

REGISTER_CLASS(WhittedIntegrator, "whitted")

NAMESPACE_END