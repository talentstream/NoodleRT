//
// Created by 44772 on 2024/2/5.
//

#include "base/texture.h"

NAMESPACE_BEGIN

class SolidTexture : public Texture {
public:
    explicit SolidTexture(const PropertyList &propertyList) {
        mAlbedo = propertyList.GetColor("albedo", {});
    }

    Color3f
    Evaluate(const Point2f& uv) const override {
        return mAlbedo;
    }

private:
    Color3f mAlbedo;
};

REGISTER_CLASS(SolidTexture, "solid")

NAMESPACE_END