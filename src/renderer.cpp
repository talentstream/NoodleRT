//
// Created by 44772 on 2024/1/30.
//

#include "core/renderer.h"
#include "util/parser.h"
#include "util/bitmap.h"

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

        std::vector<Float> data(400 * 400 * 3);
        for(int i = 0; i < 400; i++)
        {
            for(int j = 0; j < 400; j++)
            {
                data[(i * 400 + j) * 3 + 0] = mFramebuffer[i * 400 + j].r;
                data[(i * 400 + j) * 3 + 1] = mFramebuffer[i * 400 + j].g;
                data[(i * 400 + j) * 3 + 2] = mFramebuffer[i * 400 + j].b;
            }
        }
        BitMap::SavePNG("../test.png", 400, 400, data);
    }

    void Renderer::SetRenderCallback(std::function<void(const std::vector<Color3f> &)> callback) {
        renderCallback = std::move(callback);
    }

NAMESPACE_END


