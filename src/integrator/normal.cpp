//
// Created by 44772 on 2024/2/4.
//

#include "base/integrator.h"
#include <print>

NAMESPACE_BEGIN

    class NormalIntegrator : public Integrator {
    public:
        explicit NormalIntegrator(const PropertyList &propertyList) {
            PRINT_DEBUG_INFO("Integrator", "normal")
        }

        Color3f Li(const Ray &ray, const Aggregate &aggregate) const override {
            Interaction i;
            if (!aggregate.Intersect(ray, i)) {
                return {0.5f, 0.7f, 1.0f};
            }

            return Color3f{i.n.x + 1, i.n.y + 1, i.n.z + 1} * 0.5f;
        }
    };

    REGISTER_CLASS(NormalIntegrator, "normal")

NAMESPACE_END