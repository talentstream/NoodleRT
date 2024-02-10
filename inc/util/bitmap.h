//
// Created by 44772 on 2024/1/31.
//

#pragma once

#include "core/common.h"
#include <vector>
#include <string_view>

NAMESPACE_BEGIN

class BitMap {
public:
    static void SaveEXR(std::string_view filename);

    static void SavePNG(std::string_view filename, Integer width, Integer height, std::vector<Float> data);
};

NAMESPACE_END
