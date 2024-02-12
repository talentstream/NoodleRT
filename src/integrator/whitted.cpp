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
        if (!pAggregate->Intersect(ray, si)) {
            Vector3f unitDir = Normalize(ray.d);
            Float t = 0.5f * (unitDir.y + 1.0f);
            return (1.0f - t) * Color3f{1.0f, 1.0f, 1.0f} + t * Color3f{0.5f, 0.7f, 1.0f};
        }

        // shading
        auto bxdf = si.bxdf;
        Vector3f wo = -ray.d;
        Color3f Le = bxdf->Evaluate(si,wo);

        if(depth == mMaxDepth) return Le;

        auto wi = bxdf->ComputeScattering(si, ray.d);
        if(!wi.has_value()) return Le;

        Ray scattered {si.p, wi.value()};
        return Le * Trace(scattered, depth + 1);

    }

private:
    Integer mMaxDepth;
};

REGISTER_CLASS(WhittedIntegrator, "whitted")

NAMESPACE_END