//
// Created by 44772 on 2024/2/4.
//

#pragma once

#include "base/shape.h"

NAMESPACE_BEGIN

class Triangle : public Shape {
public:
    explicit Triangle(class Mesh *mesh, Integer index);

    Boolean Intersect(const Ray &ray, Float tMax, SurfaceInteraction &si) const override;

    Bound3f BoundingBox() const override;

private:
    Mesh *pMesh{nullptr};
    Integer mIndices[3];
};

NAMESPACE_END