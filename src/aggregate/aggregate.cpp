//
// Created by talentstream on 2024/3/4.
//

#include "base/aggregate.h"

NAMESPACE_BEGIN

void
aggregate::AddChild(Object *child) {

    switch (child->GetClassType()) {
        case EClassType::EShape:
            AddShape(dynamic_cast<shape *>(child));
            break;
        default:
            // throw exception
            break;
    }
}

void
aggregate::Initialize() {
    Build();
}

UInt32
aggregate::GetPrimitiveCount() const {
    return mShapeOffset.back();
}

void
aggregate::AddShape(shape *pShape) {
    mShapes.push_back(pShape);
    mShapeOffset.push_back(mShapeOffset.back() + pShape->GetPrimitiveCount());
    mBbox.Expand(pShape->GetBoundingBox());
}

UInt32
aggregate::FindShape(UInt32 &idx) const {
    auto it = std::lower_bound(mShapeOffset.begin(), mShapeOffset.end(), idx + 1) - 1;
    idx -= *it;
    return it - mShapeOffset.begin();
}

Bound3f
aggregate::FindShapeBoundingBox(UInt32 idx) const {
    auto shapeIdx = FindShape(idx);
    return mShapes[shapeIdx]->GetBoundingBox();
}

Point3f
aggregate::FindShapeCentroid(UInt32 idx) const {
    auto shapeIdx = FindShape(idx);
    return mShapes[shapeIdx]->GetCentroid();
}

NAMESPACE_END