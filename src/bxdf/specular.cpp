//
// Created by 44772 on 2024/2/12.
//

#include "base/bxdf.h"
#include <print>

NAMESPACE_BEGIN

// Ideal Specular
// Perfect reflection
class Specular : public BxDF {
public:
    explicit Specular(const PropertyList &propertyList) {
        mAlbedo = propertyList.GetColor("albedo", {0.5f, 0.5, 0.5f});
        PRINT_DEBUG_INFO("BxDF", "specular")
    }

private:
    Color3f mAlbedo;
};

REGISTER_CLASS(Specular, "specular")
NAMESPACE_END