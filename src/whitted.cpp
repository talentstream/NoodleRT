//
// Created by 44772 on 2024/1/31.
//

#include "base/integrator.h"
#include <print>

NAMESPACE_BEGIN

    class WhittedIntegrator : public Integrator {
    public:
        explicit WhittedIntegrator(const PropertyList &propList) {
            std::print("->WhittedIntegrator\n");
        }

        Color3f Li(const Ray &ray, Integer depth) const override {
            return {0.5f,0.7f,1.0f};
        }

    private:
    };

    REGISTER_CLASS(WhittedIntegrator, "whitted")

NAMESPACE_END