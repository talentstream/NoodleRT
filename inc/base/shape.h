//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/object.h"
#include "core/record.h"

NAMESPACE_BEGIN

class Shape : public Object {
public:
    virtual
    ~Shape() = default;

    virtual Boolean
    Intersect(UInt32 idx, const Ray &ray, IntersectionRecord &si) const = 0;

    virtual Boolean
    IntersectP(UInt32 idx, const Ray &ray, Float tMax = Infinity) const = 0;

    virtual void
    Sample(ShapeRecord &sRec, const Point2f &sample) const = 0;

    virtual Float
    Pdf(const ShapeRecord &sRec) const = 0;

    virtual Float
    Area() const = 0;

    virtual UInt32
    GetPrimitiveCount() const = 0;

    const Bound3f
    &GetBoundingBox() const {
        return mBbox;
    }

    Boolean
    IsEmitter() const {
        return pEmitter != nullptr;
    }

    virtual Bound3f
    GetBoundingBox(UInt32 idx) const = 0;

    virtual Point3f
    GetCentroid(UInt32 idx) const = 0;

    const BxDF
    *GetBxDF() const {
        return pBxDF;
    }

    Emitter
    *GetEmitter() const {
        return pEmitter;
    }

    void
    AddChild(Object *child) override;

    void
    Initialize() override;

    EClassType
    GetClassType() const override {
        return EClassType::EShape;
    }

protected:
    BxDF *pBxDF{nullptr};
    Emitter *pEmitter{nullptr};
    Bound3f mBbox;
};

NAMESPACE_END