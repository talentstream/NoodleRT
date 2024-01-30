//
// Created by 44772 on 2024/1/24.
//

#pragma once

#include "core/common.h"
#include <functional>

NAMESPACE_BEGIN

class Parallel {
public:
    static void For1D(Integer start, Integer end, const std::function<void(Integer)> &func);

    static void For2D(Integer startX, Integer endX, Integer startY, Integer endY,
                      const std::function<void(Integer, Integer)> &func);
};

NAMESPACE_END