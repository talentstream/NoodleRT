//
// Created by 44772 on 2024/1/31.
//

#include "base/integrator.h"
#include "base/aggregate.h"
#include "base/camera.h"
#include "base/bxdf.h"
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
        Color3f backgroundColor;
        {
            Vector3f unitDir = Normalize(ray.d);
            Float t = 0.5f * (unitDir.y + 1.0f);
            backgroundColor = (1.0f - t) * Color3f{1.0f, 1.0f, 1.0f} + t * Color3f{0.5f, 0.7f, 1.0f};
        }
        if (!pAggregate->Intersect(ray, si)) {
            return backgroundColor;
        }

        // shading
        auto bxdf = si.bxdf;
        Vector3f wo = -ray.d, wi;
        auto Le = bxdf->Sample(si, wo, wi, Point2f{});
        if(!Le.has_value()) return backgroundColor;
        auto le = Le.value();
//        Color3f oldLe = bxdf->Evaluate(si, wo);
        if (depth == mMaxDepth) return le;

//        auto oldWi = bxdf->ComputeScattering(si, ray.d);
//        if (!oldWi.has_value()) return oldLe;

        Ray scattered{si.p, wi};
        return le * Trace(scattered, depth + 1);

    }

private:
    Integer mMaxDepth;
};

REGISTER_CLASS(WhittedIntegrator, "whitted")

NAMESPACE_END