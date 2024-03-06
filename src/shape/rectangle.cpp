//
// Created by talentstream on 2024/3/4.
//

#include "base/shape.h"
#include "base/bxdf.h"
#include "base/light.h"
#include "triangle.h"
#include <print>

NAMESPACE_BEGIN

class Rectangle : public Shape {
public:
    explicit
    Rectangle(const PropertyList &propertyList) {
        mObjectToWorld *= propertyList.GetTransform("scale", {});
        mObjectToWorld *= propertyList.GetTransform("rotate", {});
        mObjectToWorld *= propertyList.GetTransform("translate", {});

        mObjectToWorld = propertyList.GetTransform("toWorld", mObjectToWorld);

        PRINT_DEBUG_INFO("Rectangle", "rectangle")
    }

    Boolean
    Intersect(UInt32 idx, const Ray &ray, IntersectionRecord &iRec) const override {
        Float u, v, t;
        auto tri = mTriangles[idx];
        if (!tri.Intersect(ray, iRec.t, u, v, t)) {
            return false;
        }
        Normal3f outNormal;
        if (tri.hasNormal) {
            outNormal = Normalize((1 - u - v) * tri.n0 + u * tri.n1 + v * tri.n2);
        } else {
            outNormal = Normalize(Normal3f{Cross(tri.p1 - tri.p0, tri.p2 - tri.p0)});
        }
        iRec = IntersectionRecord(t, ray(t), outNormal);
        iRec.SetFlipNormal(ray.d);
        if (tri.hasUV) {
            iRec.uv = (1 - u - v) * tri.uv0 + u * tri.uv1 + v * tri.uv2;
        } else {
            iRec.uv = Point2f{u + v, v};
        }
        iRec.bxdf = pBxDF;
        iRec.emitter = pEmitter;
        return true;
    }

    Boolean
    IntersectP(UInt32 idx, const Ray &ray, Float tMax = Infinity) const override {
        return mTriangles[idx].IntersectP(ray, tMax);
    }

    void
    Sample(ShapeRecord &sRec, const Point2f &sample) const override {
        auto idx = static_cast<UInt32>(sample.x * mTriangles.size());
        mTriangles[idx].Sample(sRec, sample);
    }

    Float
    Pdf(const ShapeRecord &sRec) const override {
        return 0.f;
    }

    Float
    Area() const override {
        return 0.f;
    }

    UInt32
    GetPrimitiveCount() const override {
        return mTriangles.size();
    }

    Bound3f
    GetBoundingBox(UInt32 idx) const override {
        return mTriangles[idx].GetBoundingBox();
    }

    Point3f
    GetCentroid(UInt32 idx) const override {
        return mTriangles[idx].GetCentroid();
    }

    void
    Initialize() override {
        Shape::Initialize();
        LoadRectangle();
    }

private:
    void
    LoadRectangle() {
        std::vector<Point3f> positions = {{-1, -1, 0},
                                          {1,  -1, 0},
                                          {1,  1,  0},
                                          {-1, 1,  0}};
        std::ranges::transform(positions, positions.begin(), [&](auto &p) {
            return mObjectToWorld(p);
        });
//        Normal3f n = Normalize(Normal3f{mObjectToWorld(Point3f{0, 0, 1})});
        mTriangles.emplace_back(positions[0], positions[1], positions[2]);
//        mTriangles[0].SetNormal(n, n, n);
        mTriangles.emplace_back(positions[2], positions[3], positions[0]);
//        mTriangles[1].SetNormal(n, n, n);
//        std::vector<Point2f> uvs = {{0, 0},
//                                    {1, 0},
//                                    {1, 1},
//                                    {0, 1}};
//        mTriangles[0].SetUV(uvs[0], uvs[1], uvs[2]);
//        mTriangles[1].SetUV(uvs[2], uvs[3], uvs[0]);

    }

    std::vector<Triangle> mTriangles;
    Transform mObjectToWorld;
};

REGISTER_CLASS(Rectangle, "rectangle")

NAMESPACE_END