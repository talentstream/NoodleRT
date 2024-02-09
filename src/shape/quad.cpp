//
// Created by 44772 on 2024/2/9.
//

#include "base/shape.h"
#include <print>

NAMESPACE_BEGIN

    class Quad : public Shape {
    public:
        explicit Quad(const PropertyList &propertyList) {
            mStart = propertyList.GetPoint("start", {});
            mHorizontal = propertyList.GetVector("horizontal", {1, 0, 0});
            mVertical = propertyList.GetVector("vertical", {0, 1, 0});
            PRINT_DEBUG_INFO("Shape", "quad")
        }

        Boolean Intersect(const Ray &ray, Float tMax, Interaction &i) const override {
            Vector3f vNormal = Cross(mHorizontal, mVertical);
            Vector3f vUnitNormal = Normalize(vNormal);
            if (AbsDot(ray.d, vNormal) < Epsilon) {
                return false;
            }

            auto t = Dot(mStart - ray.o, vNormal) / Dot(ray.d, vNormal);
            if (t < 0 || t > tMax) {
                return false;
            }

            Point3f p = ray(t);
            Vector3f pq = p - mStart;

            auto alpha = Dot(vUnitNormal, Cross(mHorizontal, pq));
            auto beta = Dot(vUnitNormal, Cross(pq, mVertical));
            if (alpha < 0 || alpha > 1 || beta < 0 || beta > 1) {
                return false;
            }

            i.t = t;
            i.p = p;
            i.front = Dot(ray.d, vUnitNormal) < 0;
            i.n = i.front ? Normal3f{vUnitNormal} : Normal3f{-vUnitNormal};
            i.u = alpha;
            i.v = beta;
            return false;
        }

        Bound3f BoundingBox() const override {
            return Padding(Bound3f{mStart, mStart + mHorizontal + mVertical}, Epsilon);
        }

    private:
        Point3f mStart;
        Vector3f mHorizontal, mVertical;
    };

    REGISTER_CLASS(Quad, "quad")

NAMESPACE_END