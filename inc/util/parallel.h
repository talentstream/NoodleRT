//
// Created by 44772 on 2024/1/24.
//

#pragma once

#include "core/common.h"
#include <functional>

NAMESPACE_BEGIN

class Parallel {
public:
    static void For1D(Int start, Int end, const std::function<void(Int)> &func);

    static void For2D(Int startX, Int endX, Int startY, Int endY,
                      const std::function<void(Int, Int)> &func);
};

NAMESPACE_END