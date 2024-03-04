//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/object.h"
#include "core/interaction.h"
#include "core/record.h"

NAMESPACE_BEGIN

class Shape : public Object {
public:
    virtual ~Shape() = default;

    virtual Boolean
    Intersect(const Ray &ray, Float tMax, SurfaceInteraction &) const = 0;

    virtual Boolean
    IntersectP(const Ray &ray, Float tMax) const = 0;

    virtual Bound3f
    BoundingBox() const = 0;

    virtual void
    Sample(ShapeSampleRecord &sRec, const Point2f &sample) const = 0;

    virtual Float
    Pdf(const ShapeSampleRecord &sRec) const { return 1 / Area(); };

    virtual Float
    Area() const = 0;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::EShape;
    }
};

class shape : public Object {
public:
    virtual
    ~shape() = default;

    virtual Boolean
    Intersect(UInt32 idx, const Ray &ray, Float tMax, SurfaceInteraction &si) const = 0;

    virtual Boolean
    IntersectP(UInt32 idx, const Ray &ray, Float tMax = Infinity) const = 0;

    virtual void
    Sample(ShapeSampleRecord &sRec, const Point2f &sample) const = 0;

    virtual Float
    Pdf(const ShapeSampleRecord &sRec) const = 0;

    virtual Float
    Area() const = 0;

    virtual UInt32
    GetPrimitiveCount() const = 0;

    const Bound3f
    &GetBoundingBox() const {
        return mBbox;
    }

    virtual Bound3f
    GetBoundingBox(UInt32 idx) const = 0;

    virtual Point3f
    GetCentroid(UInt32 idx) const = 0;

    const BxDF
    *GetBxDF() const {
        return pBxDF;
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
    Light *pLight{nullptr};
    Bound3f mBbox;
};

NAMESPACE_END