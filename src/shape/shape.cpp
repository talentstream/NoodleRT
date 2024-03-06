//
// Created by talentstream on 2024/3/4.
//

#include "base/shape.h"
#include "base/bxdf.h"
#include "base/light.h"

NAMESPACE_BEGIN

void
Shape::AddChild(Object *child) {
    switch (child->GetClassType()) {
        case EClassType::EBxDF:
            pBxDF = dynamic_cast<BxDF *>(child);
            break;
        case EClassType::ELight:
            pEmitter = dynamic_cast<Emitter *>(child);
            break;
        default:
            // throw exception
            break;
    }
}

void
Shape::Initialize() {
    if(pBxDF == nullptr) {
        pBxDF = dynamic_cast<BxDF *>(ObjectFactory::CreateInstance("diffuse", PropertyList(), true));
    }
}
NAMESPACE_END