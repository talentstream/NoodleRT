//
// Created by 44772 on 2024/1/31.
//

#include "base/integrator.h"
#include "core/interaction.h"
#include "base/bxdf.h"
#include <print>

NAMESPACE_BEGIN

class WhittedIntegrator : public Integrator {
public:
    explicit WhittedIntegrator(const PropertyList &propertyList) {
        mMaxDepth = propertyList.GetInteger("depth", 1);
        PRINT_DEBUG_INFO("Integrator", "whitted")
    }

    [[nodiscard]] Color3f Li(const Ray &ray) const override {
        return Trace(ray, *pAggregate, mMaxDepth);
    }

private:
    Color3f Trace(const Ray &ray, const Aggregate &aggregate, Integer depth) const {
        if (depth < 0) return {0, 0, 0};

        // find nearest intersection
        SurfaceInteraction ni;
        if (!aggregate.Intersect(ray, ni)) {
            return {0.235294, 0.67451, 0.843137};
        }

        // shading
        Color3f Le{};

        // calculate light emission

        // calculate light reflection
        Ray wo;
        Color3f attenuation;
        Boolean isSpecular = ni.bxdf->ComputeScattering(ray, ni, attenuation, wo);
        Le += attenuation;
        if (isSpecular) {
            Le += Trace(wo, aggregate, depth - 1);
        }
        return Le;
    }

private:
    Integer mMaxDepth;
};

REGISTER_CLASS(WhittedIntegrator, "whitted")

NAMESPACE_END