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

            Interaction i;
            if (!aggregate.Intersect(ray, i)) {
                return {0.5f, 0.7f, 1.0f};
            }

            Ray wo;
            Color3f attenuation;

            if (i.bxdf->ComputeScattering(ray, i, attenuation, wo)) {
                return attenuation * Trace(wo, aggregate, depth - 1);
            }

            return {0, 0, 0};
        }

    private:
        Integer mMaxDepth;
    };

    REGISTER_CLASS(WhittedIntegrator, "whitted")

NAMESPACE_END