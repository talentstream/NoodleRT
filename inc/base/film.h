//
// Created by 44772 on 2024/2/11.
//

#pragma once

#include "core/object.h"
#include <vector>
#include <ranges>

NAMESPACE_BEGIN

class Film : public Object {
public:
    explicit Film(const PropertyList &propertyList) {
        width = propertyList.GetInt("width", 400);
        height = propertyList.GetInt("height", 400);
        mSaveFileName = propertyList.GetString("saveFileName", "output.png");
        framebuffer.resize(width * height);
        PRINT_DEBUG_INFO("Film", "film")
    }

    virtual ~Film() = default;

    void Update(Int index, Color3f c) {
        framebuffer[index] += c;
    }

    // justForApplication UI Display
    void SetCallBack(std::function<void(const std::vector<Color3f> &)> callback) {
        mCallback = std::move(callback);
    }

    void Display(const Int spp) const {
        std::vector<Color3f> displayBuffer(framebuffer.size());
        std::ranges::transform(framebuffer, displayBuffer.begin(), [&](Color3f c) {
            return (c / spp).LinearToGamma();
        });

        mCallback(displayBuffer);
    }

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::EFilm;
    }


public:
    Int width;
    Int height;
    std::vector<Color3f> framebuffer;
private:
    std::function<void(const std::vector<Color3f> &)> mCallback;

    std::string_view mSaveFileName;
};

NAMESPACE_END