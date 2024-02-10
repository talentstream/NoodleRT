//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/object.h"
#include "base/aggregate.h"

NAMESPACE_BEGIN

class Integrator : public Object {
public:
    virtual ~Integrator() = default;

//    virtual void Render() const final;

    [[nodiscard]] virtual Color3f Li(const Ray &ray) const = 0;

    virtual void AddChild(Object *child) {
        switch (child->GetClassType()) {
            case EClassType::EAggregate:
                pAggregate = dynamic_cast<Aggregate *>(child);
                break;
            default:
                break;
        }
    }

    virtual void Initialize() override {
        if (pAggregate == nullptr) {
            throw std::runtime_error("Integrator Need Aggregate!");
        }
    }

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::EIntegrator;
    }

protected:
    Aggregate *pAggregate{nullptr};
    // std::vector<Light> mLights;

    // Camera *pCamera{nullptr};
    // Sampler *pSampler{nullptr};
};


NAMESPACE_END
