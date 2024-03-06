//
// Created by 44772 on 2024/1/31.
//

#pragma once

#include "core/object.h"
#include "core/record.h"
#include "base/shape.h"

NAMESPACE_BEGIN

class Aggregate : public Object {
public:
    virtual
    ~Aggregate() = default;

    virtual Boolean
    Intersect(const Ray &ray, IntersectionRecord &si) const = 0;

    virtual Boolean
    UnOccluded(const Ray &ray) const = 0;

    void
    AddShape(Shape *shape);

    void
    Initialize() override;

    EClassType
    GetClassType() const override {
        return EClassType::EAggregate;
    }

protected:
    virtual
    void Build() = 0;

    UInt32
    GetPrimitiveCount() const;

    // return shapeIdx & new idx in shape
    virtual UInt32
    FindShape(UInt32 &idx) const;

    // just idx is ok, not shape idx
    virtual Bound3f
    FindShapeBoundingBox(UInt32 idx) const;

    // just idx is ok, not shape idx
    virtual Point3f
    FindShapeCentroid(UInt32 idx) const;

    std::vector<Shape *> mShapes;
    std::vector<UInt32> mShapeOffset;
    std::vector<UInt32> mShapeIndices;
    Bound3f mBbox;
};

NAMESPACE_END