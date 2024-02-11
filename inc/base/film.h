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
        width = propertyList.GetInteger("width", 400);
        height = propertyList.GetInteger("height", 400);
        mSaveFileName = propertyList.GetString("saveFileName", "output.png");
        framebuffer.resize(width * height);
        PRINT_DEBUG_INFO("Film", "film")
    }

    virtual ~Film() = default;

    void Update(Integer index, Color3f color) {
        framebuffer[index] = color;
    }

    // justForApplication UI Display
    void SetCallBack(std::function<void(const std::vector<Color3f> &)> callback) {
        mCallback = std::move(callback);
    }

    void Display() const {
        std::vector<Color3f> displayBuffer(framebuffer.size());
        std::ranges::transform(framebuffer, displayBuffer.begin(), [&](const Color3f &color) {
            return color.LinearToGamma();
        });
        mCallback(displayBuffer);
    }

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::EFilm;
    }


public:
    Integer width;
    Integer height;
    std::vector<Color3f> framebuffer;
private:
    std::function<void(const std::vector<Color3f> &)> mCallback;

    std::string_view mSaveFileName;
};

NAMESPACE_END