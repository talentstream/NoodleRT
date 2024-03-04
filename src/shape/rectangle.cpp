//
// Created by talentstream on 2024/3/4.
//

#include "base/mesh.h"
#include "base/bxdf.h"
#include "base/light.h"
#include "triangle.h"
#include <print>

NAMESPACE_BEGIN

class Rectangle : public shape {
public:
    explicit Rectangle(const PropertyList &propertyList) {
        mObjectToWorld *= propertyList.GetTransform("scale", {});
        mObjectToWorld *= propertyList.GetTransform("rotate", {});
        mObjectToWorld *= propertyList.GetTransform("translate", {});

        mObjectToWorld = propertyList.GetTransform("toWorld", mObjectToWorld);

        PRINT_DEBUG_INFO("Rectangle", "rectangle")
    }

    Boolean
    Intersect(UInt32 idx, const Ray &ray, Float tMax, SurfaceInteraction &si) const override {
        Float u, v, t;
        auto tri = mTriangles[idx];
        Boolean hit = tri.Intersect(ray, tMax, u, v, t);
        if (!hit) return false;
        Normal3f outNormal;
        if (tri.hasNormal) {
            outNormal = Normalize((1 - u - v) * tri.n0 + u * tri.n1 + v * tri.n2);
        } else {
            outNormal = Normalize(Normal3f{Cross(tri.p1 - tri.p0, tri.p2 - tri.p0)});
        }
        si = SurfaceInteraction(t, ray(t), outNormal, -ray.d);
        if (tri.hasUV) {
            si.u = (1 - u - v) * tri.uv0.x + u * tri.uv1.x + v * tri.uv2.x;
            si.v = (1 - u - v) * tri.uv0.y + u * tri.uv1.y + v * tri.uv2.y;
        } else {
            si.u = (1 - u - v) * 0 + u * 1 + v * 1;
            si.v = (1 - u - v) * 0 + u * 0 + v * 1;
        }
        si.bxdf = pBxDF;
        si.areaLight = pLight;
        return true;
    }

    Boolean
    IntersectP(UInt32 idx, const Ray &ray, Float tMax = Infinity) const override {
        return mTriangles[idx].IntersectP(ray, tMax);
    }

    void
    Sample(ShapeSampleRecord &sRec, const Point2f &sample) const override {

    }

    Float
    Pdf(const ShapeSampleRecord &sRec) const override {
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
        shape::Initialize();
        LoadRectangle();
    }

private:
    void LoadRectangle() {
        std::vector<Point3f> positions = {{-1, -1, 0},
                                          {1,  -1, 0},
                                          {1,  1,  0},
                                          {-1, 1,  0}};
        std::ranges::transform(positions, positions.begin(), [&](auto &p) {
            return mObjectToWorld(p);
        });
        Normal3f n{0, 0, 1};
        mTriangles.emplace_back(positions[0], positions[1], positions[2]);
        mTriangles[0].SetNormal(n, n, n);
        mTriangles.emplace_back(positions[2], positions[3], positions[0]);
        mTriangles[1].SetNormal(n, n, n);
        std::vector<Point2f> uvs = {{0, 0},
                                    {1, 0},
                                    {1, 1},
                                    {0, 1}};
        mTriangles[0].SetUV(uvs[0], uvs[1], uvs[2]);
        mTriangles[1].SetUV(uvs[2], uvs[3], uvs[0]);
    }

    std::vector<triangle> mTriangles;
    Transform mObjectToWorld;
};

REGISTER_CLASS(Rectangle, "rectangle")

NAMESPACE_END