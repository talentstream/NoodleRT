//
// Created by 44772 on 2024/2/9.
//

#include "base/shape.h"
#include <print>

NAMESPACE_BEGIN

class Quad : public Shape {
public:
    explicit Quad(const PropertyList &propertyList) {
        q = propertyList.GetPoint("start", {});
        u = propertyList.GetVector("horizontal", {1, 0, 0});
        v = propertyList.GetVector("vertical", {0, 1, 0});
        PRINT_DEBUG_INFO("Shape", "quad")
        auto n = Cross(u, v);
        unitN = Normalize(n);
        d = Dot(n, Vector3f{q});
        w = Normalize(n);
    }

    Boolean Intersect(const Ray &ray, Float tMax, Interaction &i) const override {
        auto denom = Dot(ray.d, unitN);
        if (Abs(denom) < Epsilon) {
            return false;
        }

        auto t = (d - Dot(unitN, Vector3f{ray.o})) / denom;
        if (t < 0.001 || t > tMax) {
            return false;
        }

        Point3f p = ray(t);
        Vector3f pq = p - q;

        auto alpha = Dot(w, Cross(pq, v));
        auto beta = Dot(w, Cross(u, pq));
        if (alpha < 0 || alpha > 1 || beta < 0 || beta > 1) {
            return false;
        }

        i.t = t;
        i.p = p;
        i.front = Dot(ray.d, w) < 0;
        i.n = i.front ? Normal3f{w} : Normal3f{-w};
        i.u = alpha;
        i.v = beta;

        return true;
    }

    [[nodiscard]] Bound3f BoundingBox() const override {
        return Padding(Bound3f{q, q + u + v}, Epsilon);
    }

private:
    Point3f q;
    Vector3f u, v;
    // quad
    // q as start point
    // u as horizontal vector
    // v as vertical vector
    Vector3f unitN;
    Float d;
    Vector3f w;
};

REGISTER_CLASS(Quad, "quad")

NAMESPACE_END