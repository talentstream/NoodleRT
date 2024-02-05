//
// Created by 44772 on 2024/2/5.
//

#include "base/texture.h"

NAMESPACE_BEGIN

    class CheckerTexture : public Texture {
    public:
        explicit CheckerTexture(const PropertyList &propertyList) {
            mInvScale = 1 / propertyList.GetFloat("invScale", {0.1f});
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
                propertyList.SetColor("albedo", {1.0f, 1.0f, 1.0f});

                pEven = dynamic_cast<Texture *>(ObjectFactory::CreateInstance("solid", propertyList));

            }
            if (pOdd == nullptr) {
                PropertyList propertyList;
                propertyList.SetColor("albedo", {0.0f, 0.0f, 0.0f});
                pOdd = dynamic_cast<Texture *>(ObjectFactory::CreateInstance("solid", propertyList));
            }
        }

        [[nodiscard]] Color3f Evaluate(const Interaction &i) const override {
            auto x = Floor(mInvScale * i.p.x);
            auto y = Floor(mInvScale * i.p.y);
            auto z = Floor(mInvScale * i.p.z);
            if ((x + y + z) % 2 == 0) {
                return pEven->Evaluate(i);
            } else {
                return pOdd->Evaluate(i);
            }
        }

    private:
        Float mInvScale;
        Texture *pEven{nullptr};
        Texture *pOdd{nullptr};
    };

    REGISTER_CLASS(CheckerTexture, "checker")

NAMESPACE_END