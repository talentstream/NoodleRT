//
// Created by talentstream on 2024/3/4.
//

#include "base/shape.h"
#include "base/bxdf.h"
#include "base/light.h"
#include "triangle.h"

NAMESPACE_BEGIN

class Cube : public shape {
public:
    explicit
    Cube(const PropertyList &propertyList) {
        mObjectToWorld *= propertyList.GetTransform("scale", {});
        mObjectToWorld *= propertyList.GetTransform("rotate", {});
        mObjectToWorld *= propertyList.GetTransform("translate", {});

        mObjectToWorld = propertyList.GetTransform("toWorld", mObjectToWorld);

        PRINT_DEBUG_INFO("Cube", "cube")
    }

    Boolean
    Intersect(UInt32 idx, const Ray &ray, Float tMax, IntersectionRecord &si) const override {
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
        si = IntersectionRecord(t, ray(t), outNormal);
        si.SetFlipNormal(ray.d);
        if (tri.hasUV) {
            si.uv = (1 - u - v) * tri.uv0 + u * tri.uv1 + v * tri.uv2;
        } else {
            si.uv = Point2f{u + v, v};
        }
        si.bxdf = pBxDF;
        si.emitter = pEmitter;
        return true;
    }

    Boolean
    IntersectP(UInt32 idx, const Ray &ray, Float tMax = Infinity) const override {
        return mTriangles[idx].IntersectP(ray, tMax);
    }

    void
    Sample(ShapeRecord &sRec, const Point2f &sample) const override {

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
        shape::Initialize();
        LoadCube();
    }

private:
    void
    LoadCube() {
        std::vector<Point3f> positions = {
                {1,  -1, -1},
                {1,  -1, 1},
                {-1, -1, 1},
                {-1, -1, -1},
                {1,  1,  -1},
                {-1, 1,  -1},
                {-1, 1,  1},
                {1,  1,  1},
                {1,  -1, -1},
                {1,  1,  -1},
                {1,  1,  1},
                {1,  -1, 1},
                {1,  -1, 1},
                {1,  1,  1},
                {-1, 1,  1},
                {-1, -1, 1},
                {-1, -1, 1},
                {-1, 1,  1},
                {-1, 1,  -1},
                {-1, -1, -1},
                {1,  1,  -1},
                {1,  -1, -1},
                {-1, -1, -1},
                {-1, 1,  -1}
        };
        std::ranges::transform(positions, positions.begin(), [&](auto &p) {
            return mObjectToWorld(p);
        });
//        std::vector<Normal3f> normals = {
//                {0,  -1, 0},
//                {0,  -1, 0},
//                {0,  -1, 0},
//                {0,  -1, 0},
//                {0,  1,  0},
//                {0,  1,  0},
//                {0,  1,  0},
//                {0,  1,  0},
//                {1,  0,  0},
//                {1,  0,  0},
//                {1,  0,  0},
//                {1,  0,  0},
//                {0,  0,  1},
//                {0,  0,  1},
//                {0,  0,  1},
//                {0,  0,  1},
//                {-1, 0,  0},
//                {-1, 0,  0},
//                {-1, 0,  0},
//                {-1, 0,  0},
//                {0,  0,  -1},
//                {0,  0,  -1},
//                {0,  0,  -1},
//                {0,  0,  -1}
//        };
//        std::ranges::transform(normals, normals.begin(), [&](auto &n) {
//            return Normalize(Normal3f{mObjectToWorld(Point3f{n.x, n.y, n.z})});
//        });
//        std::vector<Point2f> uvs = {
//                {0, 1},
//                {1, 1},
//                {1, 0},
//                {0, 0},
//                {0, 1},
//                {1, 1},
//                {1, 0},
//                {0, 0},
//                {0, 1},
//                {1, 1},
//                {1, 0},
//                {0, 0},
//                {0, 1},
//                {1, 1},
//                {1, 0},
//                {0, 0},
//                {0, 1},
//                {1, 1},
//                {1, 0},
//                {0, 0},
//                {0, 1},
//                {1, 1},
//                {1, 0},
//                {0, 0}
//        };

        std::vector<Point3i> indices = {
                {0,  1,  2},
                {3,  0,  2},
                {4,  5,  6},
                {7,  4,  6},
                {8,  9,  10},
                {11, 8,  10},
                {12, 13, 14},
                {15, 12, 14},
                {16, 17, 18},
                {19, 16, 18},
                {20, 21, 22},
                {23, 20, 22}
        };

        for (const auto idx: indices) {
            auto idx0 = idx[0], idx1 = idx[1], idx2 = idx[2];
            mTriangles.emplace_back(positions[idx0], positions[idx1], positions[idx2]);
//            mTriangles.back().SetNormal(normals[idx0], normals[idx1], normals[idx2]);
//            mTriangles.back().SetUV(uvs[idx0], uvs[idx1], uvs[idx2]);
        }
    }

    std::vector<triangle> mTriangles;
    Transform mObjectToWorld;
};


REGISTER_CLASS(Cube, "cube")

NAMESPACE_END