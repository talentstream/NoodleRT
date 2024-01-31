//
// Created by 44772 on 2024/1/31.
//

#include "base/integrator.h"
#include "base/primitive.h"
#include "core/interaction.h"
#include <print>

NAMESPACE_BEGIN

    class WhittedIntegrator : public Integrator {
    public:
        explicit WhittedIntegrator(const PropertyList &propertyList) {
            PRINT_DEBUG_INFO("Integrator", "whitted")
        }

        Color3f Li(const Ray &ray, const Aggregate &aggregate, Integer depth) const override {
            Interaction i;
            if (!aggregate.Intersect(ray, i)) {
                return {0.5f, 0.7f, 1.0f};
            }
            return {1.0f, 0.0f, 0.0f};
        }

    private:
    };

    REGISTER_CLASS(WhittedIntegrator, "whitted")

NAMESPACE_END