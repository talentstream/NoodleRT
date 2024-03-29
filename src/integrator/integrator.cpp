//
// Created by 44772 on 2024/2/10.
//

#include "base/integrator.h"
#include "base/aggregate.h"
#include "base/camera.h"
#include "base/film.h"
#include "base/sampler.h"
#include "base/emitter.h"
#include "util/parallel.h"
#include "util/timer.h"
#include <ranges>

NAMESPACE_BEGIN

Integrator::Integrator(const PropertyList &propertyList) {
    mSpp = propertyList.GetInteger("spp", 1);
    auto aggregateType = propertyList.GetString("aggregate", "bvh");
    pAggregate = dynamic_cast<Aggregate *>(ObjectFactory::CreateInstance(aggregateType, {}));
}

void Integrator::AddChild(Object *child) {
    switch (child->GetClassType()) {
        case EClassType::EShape: {
            const auto pShape = dynamic_cast<Shape *>(child);
            pAggregate->AddShape(pShape);
            if (pShape->IsEmitter()) {
                mLights.push_back(pShape->GetEmitter());
            }
            break;
        }
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
    pAggregate->Initialize();
}

static Integer currentSpp{0};
static Integer waveLength{1};
static Timer renderingTimer;
static Timer sppTimer;

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
            renderingTimer.Reset();
        }
        sppTimer.Reset();
        Integer startSpp = currentSpp;
        Integer endSpp = std::min(currentSpp + waveLength, mSpp);
        waveLength *= 2;
        for (Integer spp = startSpp; spp < endSpp; spp++) {
            Parallel::For2D(0, tileSizeX, 0, tileSizeY, RenderBlock);

        }
        currentSpp = endSpp;
        std::print("SPP: from {} to {} - ", startSpp, currentSpp);
        sppTimer.PrintElapsedMillSec();
        if (currentSpp == mSpp) {
            std::print("Total Rendering Time - ");
            renderingTimer.PrintElapsedMillSec();
            std::print("RENDERING END============================================\n");
        }
    }

    film->Display(currentSpp);
}

NAMESPACE_END