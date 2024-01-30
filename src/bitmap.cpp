//
// Created by 44772 on 2024/1/31.
//

#include "util/bitmap.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image_write.h>

NAMESPACE_BEGIN

    void BitMap::SaveEXR(std::string_view filename) {
        // Todo:
    }

    void BitMap::SavePNG(std::string_view filename, Integer width, Integer height, std::vector<Float> data) {
        stbi_write_png(filename.data(), width, height, 3, data.data(), width * 3);
    }

NAMESPACE_END