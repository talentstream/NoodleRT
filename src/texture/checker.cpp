//
// Created by 44772 on 2024/2/5.
//

#include "base/texture.h"
#include <print>

NAMESPACE_BEGIN

class CheckerTexture : public Texture {
public:
    explicit CheckerTexture(const PropertyList &propertyList) {
        mInvScale = 1 / propertyList.GetFloat("scale", {0.01f});
        PRINT_DEBUG_INFO("Texture", "checker")
    }

    void AddChild(Object *object) override {
        switch (object->GetClassType()) {
            case EClassType::ETexture: {
                if (pEven == nullptr) {
                    pEven = dynamic_cast<Texture *>(object);
                } else if (pOdd == nullptr) {
                    pOdd = dynamic_cast<Texture *>(object);
                }
                break;
            }
        }
    }

    void Initialize() override {
        if (pEven == nullptr) {
            PropertyList propertyList;
            propertyList.SetColor("albedo", {1});

            pEven = dynamic_cast<Texture *>(ObjectFactory::CreateInstance("solid", propertyList, true));

        }
        if (pOdd == nullptr) {
            PropertyList propertyList;
            propertyList.SetColor("albedo", {0});
            pOdd = dynamic_cast<Texture *>(ObjectFactory::CreateInstance("solid", propertyList, true));
        }
    }

    [[nodiscard]] Color3f Evaluate(const Point2f &uv) const override {
        int x = static_cast<int>(uv.x * mInvScale);
        int y = static_cast<int>(uv.y * mInvScale);
        if ((x + y) % 2 == 1) {
            return pEven->Evaluate(uv);
        } else {
            return pOdd->Evaluate(uv);
        }
    }

private:
    Float mInvScale;
    Texture *pEven{nullptr};
    Texture *pOdd{nullptr};
};

REGISTER_CLASS(CheckerTexture, "checker")

NAMESPACE_END