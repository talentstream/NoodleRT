//
// Created by 44772 on 2024/1/31.
//

#pragma once

#include "core/object.h"
#include "core/interaction.h"
#include "base/shape.h"

NAMESPACE_BEGIN

class Aggregate : public Object {
public:
    virtual
    ~Aggregate() = default;

    virtual Boolean
    Intersect(const Ray &ray, SurfaceInteraction &interaction) const = 0;

    virtual Boolean
    UnOccluded(const Ray &ray) const = 0;

    EClassType
    GetClassType() const override {
        return EClassType::EAggregate;
    }
};

class aggregate : public Object {
public:
    virtual
    ~aggregate() = default;

    virtual Boolean
    Intersect(const Ray &ray, SurfaceInteraction &interaction) const = 0;

    virtual Boolean
    UnOccluded(const Ray &ray) const = 0;

    EClassType
    GetClassType() const override {
        return EClassType::EAggregate;
    }

    void
    AddChild(Object *child) override;

protected:
    void
    AddShape(Shape *shape);

    virtual UInt32
    FindShape(UInt32 idx) const;

    virtual Bound3f
    FindShapeBoundingBox(UInt32 idx) const;

    virtual Point3f
    FindShapeCentroid(UInt32 idx) const;

    std::vector<Shape *> mShapes;
    std::vector<UInt32> mShapeOffset;
    std::vector<UInt32> mShapeIndices;
    Bound3f mBBox;
};

NAMESPACE_END