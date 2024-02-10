//
// Created by 44772 on 2024/2/10.
//

#include "base/integrator.h"
#include "base/aggregate.h"
#include "base/camera.h"
#include <ranges>

NAMESPACE_BEGIN

void Integrator::AddChild(Object *child) {
    switch (child->GetClassType()) {
        case EClassType::EAggregate:
            pAggregate = dynamic_cast<Aggregate *>(child);
            break;
        case EClassType::ECamera:
            pCamera = dynamic_cast<Camera *>(child);
            break;
        default:
            throw std::runtime_error("Integrator Add Error Child:" +
                                     Object::ClassTypeName(child->GetClassType()));
    }
}

void Integrator::Initialize() {
    if (pAggregate == nullptr) {
        throw std::runtime_error("Integrator Need Aggregate!");
    }
    if (pCamera == nullptr) {
        throw std::runtime_error("Integrator Need Camera!");
    }
}

void ImageTileIntegrator::Render() const {
    for (const Integer y: std::views::iota(0, 400)) {
        auto index = (400 - y - 1) * 400;
        for (const Integer x: std::views::iota(0, 400)) {
            auto ray = pCamera->GenerateRay(Point2f(x, y));
            auto color = Li(ray);
        }
    }

}

NAMESPACE_END