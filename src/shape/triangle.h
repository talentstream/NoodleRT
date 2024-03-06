//
// Created by 44772 on 2024/2/4.
//

#pragma once

#include "base/shape.h"

NAMESPACE_BEGIN

struct Triangle {
    Point3f p0, p1, p2;
    Normal3f n0, n1, n2;
    Boolean hasNormal{false};
    Point2f uv0, uv1, uv2;
    Boolean hasUV{false};

    Triangle(Point3f p0, Point3f p1, Point3f p2)
            : p0{p0}, p1{p1}, p2{p2} {
    }

    void
    SetNormal(Normal3f n0, Normal3f n1, Normal3f n2) {
        this->n0 = n0;
        this->n1 = n1;
        this->n2 = n2;
        hasNormal = true;
    }

    void
    SetUV(Point2f uv0, Point2f uv1, Point2f uv2) {
        this->uv0 = uv0;
        this->uv1 = uv1;
        this->uv2 = uv2;
        hasUV = true;
    }

    Bound3f
    GetBoundingBox() const {
        return Union(Bound3f{p0, p1}, p2);
    }

    Point3f
    GetCentroid() const {
        return (p0 + p1 + p2) / 3.f;
    }

    Boolean
    Intersect(const Ray &ray, Float tMax, Float &u, Float &v, Float &t) const {
        Vector3f e1 = p1 - p0, e2 = p2 - p0;
        Vector3f pVec = Cross(ray.d, e2);
        Float det = Dot(e1, pVec);
        if (Abs(det) < Epsilon) {
            return false;
        }

        Float invDet = 1.f / det;
        Vector3f tVec = ray.o - p0;
        u = Dot(tVec, pVec) * invDet;
        if (u < 0.f || u > 1.f) {
            return false;
        }
        Vector3f qVec = Cross(tVec, e1);
        v = Dot(ray.d, qVec) * invDet;
        if (v < 0.f || u + v > 1.f) {
            return false;
        }

        t = Dot(e2, qVec) * invDet;
        return t >= Epsilon && t <= tMax;
    }

    Boolean
    IntersectP(const Ray &ray, Float tMax) const {
        Float u, v, t;
        return Intersect(ray, tMax, u, v, t);
    }

};

NAMESPACE_END