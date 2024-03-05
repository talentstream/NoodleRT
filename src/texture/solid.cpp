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

    [[nodiscard]] Color3f Evaluate(const IntersectionRecord &) const override {
        return mAlbedo;
    }

private:
    Color3f mAlbedo;
};

REGISTER_CLASS(SolidTexture, "solid")

NAMESPACE_END