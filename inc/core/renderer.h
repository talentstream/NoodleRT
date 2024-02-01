//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/common.h"
#include "core/math.h"
#include <functional>

NAMESPACE_BEGIN

    class Renderer {
    public:
        NON_COPY(Renderer)

        Renderer();

        ~Renderer();

        void OnInit();

        void OnRender();

        void SetRenderCallback(std::function<void(const std::vector<Color3f> &)> callback);

    private:

        std::function<void(const std::vector<Color3f> &)> renderCallback;
        std::vector<Color3f> mFramebuffer;

        std::unique_ptr<class Scene> pScene;
        Integer mCurrentSpp{};
    };

NAMESPACE_END

