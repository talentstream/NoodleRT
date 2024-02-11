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
        return Trace(ray, mMaxDepth);
    }

private:
    Color3f Trace(const Ray &ray, Integer depth) const {
        if (depth < 0) return {0, 0, 0};

        // find nearest intersection
        SurfaceInteraction si;
        if (!pAggregate->Intersect(ray, si)) {
            // calculate infinite light or background color
            return {0.235294, 0.67451, 0.843137};
        }

        // shading
        Color3f Le{};
        auto bxdf = si.bxdf;
        // calculate light emission

        // calculate light reflection
        Ray oldWo;
        Vector3f wo = -ray.d;

        auto wi = bxdf->GenerateRay(si, wo);
        if (wi.has_value()) {
            Le += Trace(Ray(si.p, wi.value()), depth - 1);
        } else {
            Le = bxdf->Evaluate(si, wo);
        }
//        Color3f attenuation;
//
//        Boolean isSpecular = si.bxdf->ComputeScattering(ray, si, attenuation, oldWo);
//        Le += attenuation;
//        if (isSpecular) {
//            Le += Trace(oldWo, depth - 1);
//        }
        return Le;
    }

private:
    Integer mMaxDepth;
};

REGISTER_CLASS(WhittedIntegrator, "whitted")

NAMESPACE_END