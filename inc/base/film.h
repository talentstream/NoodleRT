//
// Created by 44772 on 2024/2/11.
//

#pragma once

#include "core/object.h"
#include <vector>
#include <ranges>
#include <print>
NAMESPACE_BEGIN

class Film : public Object {
public:
    explicit Film(const PropertyList &propertyList) {
        filmBound = Bound2i{{0, 0},
                            {propertyList.GetInteger("width", {}),
                                propertyList.GetInteger("height", {})}};
        mSaveFileName = propertyList.GetString("saveFileName", "output.png");
        mFramebuffer.resize(filmBound.Area());
        PRINT_DEBUG_INFO("Film", "film")
    }

    virtual ~Film() = default;

    void Display() const {
        std::vector<Color3f> displayBuffer(mFramebuffer.size());
        std::ranges::transform(mFramebuffer, displayBuffer.begin(), [&](const Color3f &color) {
            return color.LinearToGamma();
        });
    }


    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::EFilm;
    }

    std::vector<Color3f> &GetPixels() {
        return mFramebuffer;
    }

public:
    Bound2i filmBound;
private:
    std::vector<Color3f> mFramebuffer;
    std::string_view mSaveFileName;
};

NAMESPACE_END