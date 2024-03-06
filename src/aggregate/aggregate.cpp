//
// Created by talentstream on 2024/3/4.
//

#include "base/Aggregate.h"

NAMESPACE_BEGIN

void
Aggregate::AddChild(Object *child) {

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
Aggregate::Initialize() {
    Build();
}

UInt32
Aggregate::GetPrimitiveCount() const {
    return mShapeOffset.back();
}

void
Aggregate::AddShape(shape *pShape) {
    mShapes.push_back(pShape);
    mShapeOffset.push_back(mShapeOffset.back() + pShape->GetPrimitiveCount());

    mBbox.Expand(pShape->GetBoundingBox());

}

UInt32
Aggregate::FindShape(UInt32 &idx) const {
    auto it = std::lower_bound(mShapeOffset.begin(), mShapeOffset.end(), idx + 1) - 1;
    idx -= *it;
    return it - mShapeOffset.begin();
}

Bound3f
Aggregate::FindShapeBoundingBox(UInt32 idx) const {
    auto shapeIdx = FindShape(idx);
    return mShapes[shapeIdx]->GetBoundingBox(idx);
}

Point3f
Aggregate::FindShapeCentroid(UInt32 idx) const {
    auto shapeIdx = FindShape(idx);
    return mShapes[shapeIdx]->GetCentroid(idx);
}

NAMESPACE_END