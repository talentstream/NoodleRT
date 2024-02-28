//
// Created by talentstream on 2024/2/28.
//

#include "base/light.h"
#include "base/shape.h"
#include <print>

NAMESPACE_BEGIN
class AreaLight : public Light {
public:
    explicit AreaLight(const PropertyList &propList) {
        mIntensity = propList.GetColor("intensity", {1});
    }

    void AddChild(Object *child) override {
        switch (child->GetClassType()) {
            case EClassType::EShape:
                pShape = dynamic_cast<Shape *>(child);
                break;
        }
    }

private:
    Shape *pShape;
    Color3f mIntensity;
};

NAMESPACE_END