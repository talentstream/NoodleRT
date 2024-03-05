//
// Created by 44772 on 2024/2/10.
//

#include "base/integrator.h"
#include "base/aggregate.h"
#include "base/camera.h"
#include "base/film.h"
#include "base/sampler.h"
#include "base/light.h"
#include "util/parallel.h"
#include "util/timer.h"
#include <ranges>

NAMESPACE_BEGIN

void Integrator::AddChild(Object *child) {
    switch (child->GetClassType()) {
        case EClassType::EAggregate:
            pAggregate = dynamic_cast<aggregate *>(child);
            break;
        case EClassType::ECamera:
            pCamera = dynamic_cast<Camera *>(child);
            break;
        case EClassType::ESampler:
            pSampler = dynamic_cast<Sampler *>(child);
            break;
        case EClassType::ELight:
            mLights.emplace_back(dynamic_cast<Emitter *>(child));
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
    if (pSampler == nullptr) {
        throw std::runtime_error("Integrator Need Sampler!");
    }
}

static Integer currentSpp{0};
static Timer timer;

void ImageTileIntegrator::Render() const {
    const auto film = pCamera->GetFilm();
    const auto width = film->width;
    const auto height = film->height;

    static const Integer tileSizeX{8}, tileSizeY{8};
    static const Integer tileHeight = Floor(static_cast<Float>(height) / static_cast<Float>(tileSizeY));
    static const Integer tileWidth = Floor(static_cast<Float>(width) / static_cast<Float>(tileSizeX));
    static auto RenderBlock = [&](Integer tileX, Integer tileY) {
        auto beginY = tileY * tileHeight;
        auto endY = (tileY + 1) * tileHeight;
        endY = endY > height ? height : endY;
        auto beginX = tileX * tileWidth;
        auto endX = (tileX + 1) * tileWidth;
        endX = endX > width ? width : endX;

        for (const Integer y: std::views::iota(beginY, endY)) {
            auto index = (height - y - 1) * width;
            for (const Integer x: std::views::iota(beginX, endX)) {
                auto ray = pCamera->GenerateRay(Point2f(x, y), pSampler->Next2D());
                film->Update(index + x, Li(ray));
            }
        }
    };

    if (currentSpp < mSpp) {
        if (currentSpp == 0) {
            std::print("RENDERING BEGIN============================================\n");
        }
        timer.Reset();
        Parallel::For2D(0, tileSizeX, 0, tileSizeY, RenderBlock);
        currentSpp++;
        std::print("SPP: {} - ", currentSpp);
        timer.PrintElapsedMillSec();
        if (currentSpp == mSpp) {
            std::print("RENDERING END============================================\n");
        }
    }

    film->Display(currentSpp);
}

NAMESPACE_END