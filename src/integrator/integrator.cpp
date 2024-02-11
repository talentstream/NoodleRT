//
// Created by 44772 on 2024/2/10.
//

#include "base/integrator.h"
#include "base/aggregate.h"
#include "base/camera.h"
#include "base/film.h"
#include "util/parallel.h"
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
    const auto film = pCamera->GetFilm();
    const auto width = film->width;
    const auto height = film->height;

    std::vector<Color3f> framebuffer(width * height);
    const Integer tileSizeX{8}, tileSizeY{8};
    const Integer tileHeight = Floor(static_cast<Float>(height) / static_cast<Float>(tileSizeY));
    const Integer tileWidth = Floor(static_cast<Float>(width) / static_cast<Float>(tileSizeX));

    Parallel::For2D(0, tileSizeX, 0, tileSizeY, [&](Integer tileX, Integer tileY) {
        auto beginY = tileY * tileHeight;
        auto endY = (tileY + 1) * tileHeight;
        endY = endY > height ? height : endY;
        auto beginX = tileX * tileWidth;
        auto endX = (tileX + 1) * tileWidth;
        endX = endX > width ? width : endX;

        for (const Integer y: std::views::iota(beginY, endY)) {
            auto index = (height - y - 1) * width;
            for (const Integer x: std::views::iota(beginX, endX)) {
                auto ray = pCamera->GenerateRay(Point2f(x, y));
                auto L = Li(ray);
                film->Update(index + x, L);
            }
        }
    });

    film->Display();
}

NAMESPACE_END