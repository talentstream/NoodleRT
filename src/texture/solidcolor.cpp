//
// Created by 44772 on 2024/2/5.
//

#include "base/texture.h"

NAMESPACE_BEGIN

    class SolidColorTexture : public Texture {
    public:
        explicit SolidColorTexture(const PropertyList &propertyList) {
            mAlbedo = propertyList.GetColor("albedo", {});
        }

        [[nodiscard]] Color3f Value(const Interaction&) const override {
            return mAlbedo;
        }

    private:
        Color3f mAlbedo;
    };

NAMESPACE_END