//
// Created by 44772 on 2024/2/5.
//

#include "base/texture.h"
#include <print>

NAMESPACE_BEGIN

class CheckerTexture : public Texture {
public:
    explicit CheckerTexture(const PropertyList &propertyList) {
        mInvScale = 1 / propertyList.GetFloat("scale", {0.32f});
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

    [[nodiscard]] Color3f Evaluate(const SurfaceInteraction &si) const override {
        auto p = si.p;
        auto x = Floor(mInvScale * p.x);
        auto y = Floor(mInvScale * p.y);
        auto z = Floor(mInvScale * p.z);
        if ((x + y + z) % 2 == 0) {
            return pEven->Evaluate(si);
        } else {
            return pOdd->Evaluate(si);
        }
    }

private:
    Float mInvScale;
    Texture *pEven{nullptr};
    Texture *pOdd{nullptr};
};

REGISTER_CLASS(CheckerTexture, "checker")

NAMESPACE_END