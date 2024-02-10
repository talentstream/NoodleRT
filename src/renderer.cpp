//
// Created by 44772 on 2024/1/30.
//

#include "core/renderer.h"
#include "core/scene.h"
#include "base/camera.h"
#include "base/integrator.h"
#include "util/parser.h"
#include "util/bitmap.h"
#include "util/timer.h"
#include "util/parallel.h"
#include <print>
#include <ranges>

NAMESPACE_BEGIN
Renderer::Renderer() {
    Object *obj = LoadSceneXML("../twospheres.xml");
    pScene = std::unique_ptr<Scene>(static_cast<Scene *>(obj));
}

Renderer::~Renderer() = default;

void Renderer::OnInit() {
    mFramebuffer.resize(pScene->GetHeight() * pScene->GetWidth());

}

void Renderer::OnRender() {
    const Camera *pCamera = pScene->GetCamera();
    const Integrator *pIntegrator = pScene->GetIntegrator();
    const Aggregate *pAggregate = pScene->GetAggregate();
    const auto imageWidth = pScene->GetWidth();
    const auto imageHeight = pScene->GetHeight();
    Integer tileSizeX{8}, tileSizeY{8};
    Integer tileHeight = std::floor(static_cast<Float>(imageHeight) / static_cast<Float>(tileSizeY));
    Integer tileWidth = std::floor(static_cast<Float>(imageWidth) / static_cast<Float>(tileSizeX));

    auto RenderBlock = [&](Integer tileX, Integer tileY) {
        auto beginY = tileY * tileHeight;
        auto endY = (tileY + 1) * tileHeight;
        endY = endY > imageHeight ? imageHeight : endY;
        auto beginX = tileX * tileWidth;
        auto endX = (tileX + 1) * tileWidth;
        endX = endX > imageWidth ? imageWidth : endX;

        // write reverse-y
        for (const Integer y: std::views::iota(beginY, endY)) {
            auto index = (imageHeight - y - 1) * imageWidth;
            for (const Integer x: std::views::iota(beginX, endX)) {
                Ray ray = pCamera->GenerateRay(Point2f(x, y));

                Color3f L = pIntegrator->Li(ray);
                mFramebuffer[index + x] += L;
            }
        }
    };

    if (mCurrentSpp < pScene->GetSpp()) {
        Timer timer;
        Parallel::For2D(0, tileSizeX, 0, tileSizeY, RenderBlock);
        mCurrentSpp++;
        std::print("SPP: {} - ", mCurrentSpp);
        timer.PrintElapsedMillSec();
    }


    std::vector<Color3f> displayBuffer(mFramebuffer.size());
    std::ranges::transform(mFramebuffer, displayBuffer.begin(), [&](const Color3f &color) {
        return (color / mCurrentSpp).LinearToGamma();
    });

    renderCallback(displayBuffer);

//        BitMap::SavePNG("../test.png", imageHeight, imageWidth, data);
}

void Renderer::SetRenderCallback(std::function<void(const std::vector<Color3f> &)> callback) {
    renderCallback = std::move(callback);
}

NAMESPACE_END


