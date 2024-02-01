//
// Created by 44772 on 2024/1/30.
//

#include "core/renderer.h"
#include "core/scene.h"
#include "base/camera.h"
#include "base/integrator.h"
#include "util/parser.h"
#include "util/bitmap.h"
#include <print>
#include <ranges>

NAMESPACE_BEGIN
    Renderer::Renderer() {
        Object *obj = LoadSceneXML("../test.xml");
        pScene = std::unique_ptr<Scene>(static_cast<Scene *>(obj));
    }

    Renderer::~Renderer() = default;

    void Renderer::OnInit() {
        mFramebuffer.resize(400 * 400);

    }

    void Renderer::OnRender() {
        const Camera *pCamera = pScene->GetCamera();
        const Integrator *pIntegrator = pScene->GetIntegrator();
        const Aggregate *pAggregate = pScene->GetAggregate();

        if(mCurrentSpp < pScene->GetSpp())
        {
            for (int i = 0; i < 400; i++) {
                for (int j = 0; j < 400; j++) {
                    Ray ray = pCamera->GenerateRay(Point2f(j, i));
                    mFramebuffer[i * 400 + j] += pIntegrator->Li(ray, *pAggregate, 0);
                }
            }
            mCurrentSpp++;
            std::print("SPP: {} - ", mCurrentSpp);
        }


        std::vector<Color3f> displayBuffer(mFramebuffer.size());
        std::ranges::transform(mFramebuffer, displayBuffer.begin(), [&](const Color3f &color) {
            return (color / mCurrentSpp).LinearToGamma();
        });

        renderCallback(displayBuffer);

//        BitMap::SavePNG("../test.png", 400, 400, data);
    }

    void Renderer::SetRenderCallback(std::function<void(const std::vector<Color3f> &)> callback) {
        renderCallback = std::move(callback);
    }

NAMESPACE_END


