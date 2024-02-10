//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/common.h"
#include <memory>

NAMESPACE_BEGIN

class Application {
public:
    NON_COPY(Application)

    Application();

    ~Application();

    void Run();

private:
    void InitUI();

private:
    std::unique_ptr<class Renderer> pRenderer;
};

NAMESPACE_END
