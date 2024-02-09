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

        Color3f Li(const Ray &ray, const Aggregate &aggregate) const override {
            return Trace(ray, aggregate, mMaxDepth);
        }

    private:
        Color3f Trace(const Ray &ray, const Aggregate &aggregate, Integer depth) const {
            if (depth < 0) return {0, 0, 0};

            // find nearest intersection
            Interaction ni;
            if (!aggregate.Intersect(ray, ni)) {
                return {0.5f, 0.7f, 1.0f};
            }

            // shading
            Color3f Le{};

            // calculate light emission

            // calculate refection
            Ray wo;
            Color3f attenuation;

            if (ni.bxdf->ComputeScattering(ray, ni, attenuation, wo)) {
                return Le + attenuation * Trace(wo, aggregate, depth - 1);
            }
            return Le;
        }

    private:
        Integer mMaxDepth;
    };

    REGISTER_CLASS(WhittedIntegrator, "whitted")

NAMESPACE_END