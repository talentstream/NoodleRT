//
// Created by 44772 on 2024/2/4.
//

#include "base/shape.h"

NAMESPACE_BEGIN

    class Triangle : public Shape {
    public:
        Boolean Intersect(const Ray &ray, Float tMax, Interaction &i) const override {
            return true;
        }

        Bound3f BoundingBox() const override {
            return {};
        }

    private:

    };

NAMESPACE_END