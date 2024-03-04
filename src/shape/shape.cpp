//
// Created by talentstream on 2024/3/4.
//

#include "base/shape.h"
#include "base/bxdf.h"

NAMESPACE_BEGIN

void
shape::AddChild(Object *child) {
    switch (child->GetClassType()) {
        case EClassType::EBxDF:
            pBxDF = dynamic_cast<BxDF *>(child);
            break;
        default:
            // throw exception
            break;
    }
}

NAMESPACE_END