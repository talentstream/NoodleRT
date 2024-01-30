//
// Created by 44772 on 2024/1/30.
//

#include "core/renderer.h"
#include "util/parser.h"

NAMESPACE_BEGIN
    Renderer::Renderer() {
        LoadSceneXML("../test.xml");
    }

    Renderer::~Renderer() = default;

    void Renderer::OnInit() {
        mFramebuffer.resize(400 * 400);
    }

    void Renderer::OnRender() {
        for (int i = 0; i < 400; i++) {
            for (int j = 0; j < 400; j++) {
                mFramebuffer[i * 400 + j] = RandomColor3f(0, 1);
            }
        }

        renderCallback(mFramebuffer);
    }

    void Renderer::SetRenderCallback(std::function<void(const std::vector<Color3f> &)> callback) {
        renderCallback = std::move(callback);
    }

NAMESPACE_END


