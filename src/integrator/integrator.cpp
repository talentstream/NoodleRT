//
// Created by 44772 on 2024/2/10.
//

#include "base/integrator.h"
#include "base/aggregate.h"
#include "base/camera.h"
#include "base/film.h"
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

std::vector<Color3f> ImageTileIntegrator::Render() const {
    const auto film = pCamera->GetFilm();
    const auto width = film->width;
    const auto height = film->height;

    std::vector<Color3f> framebuffer(width * height);
    for (const Integer y: std::views::iota(0, height)) {
        auto index = (height - y - 1) * width;
        for (const Integer x: std::views::iota(0, width)) {
            auto ray = pCamera->GenerateRay(Point2f(x, y));
            framebuffer[index + x] = Li(ray);
        }
    }

    return framebuffer;

}

NAMESPACE_END