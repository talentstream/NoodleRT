//
// Created by 44772 on 2024/2/3.
//

#include "base/aggregate.h"
#include "base/primitive.h"
#include "base/mesh.h"
#include <print>
#include <vector>
#include <algorithm>

NAMESPACE_BEGIN

struct PrimitiveInfo {
    PrimitiveInfo() = default;

    PrimitiveInfo(Integer i, Bound3f b)
            : index{i}, bounds{b},
              centroid{0.5f * b.pMin + 0.5f * b.pMax} {}

    Integer index;
    Bound3f bounds;
    Point3f centroid;
};


struct BVHNode {
    void InitLeaf(Integer first, Integer n, Bound3f b) {
        firstPrimIndex = first;
        primitiveNumber = n;
        bound = b;
        children[0] = children[1] = nullptr;
    }

    void InitInterior(BVHNode *c0, BVHNode *c1) {
        children[0] = c0;
        children[1] = c1;
        bound = Union(c0->bound, c1->bound);
    }

    Bound3f bound;
    BVHNode *children[2];
    Integer firstPrimIndex;
    Integer primitiveNumber{0};
};


// 1. bvh construction
//    1.1 compute bounding information
//    1.2 select building info
//    1.3 build bvh
// 2. bvh traversal
class BVHAggregate : public Aggregate {
public:
    explicit BVHAggregate(const PropertyList &propertyList) {
        PRINT_DEBUG_INFO("Aggregate", "bvh")
    }

    void AddChild(Object *object) override {
        switch (object->GetClassType()) {
            case EClassType::EPrimitive:
                mPrimitives.emplace_back(dynamic_cast<Primitive *>(object));
                break;
            case EClassType::EMesh: {
                auto mesh = dynamic_cast<Mesh *>(object);
                mPrimitives.insert(mPrimitives.end(), mesh->primitives.begin(), mesh->primitives.end());
                break;
            }
            default:
                /*throw*/
                break;
        }
    }

    void Initialize() override {
        if (mPrimitives.empty()) return;
        Integer primitiveSize = mPrimitives.size();
        std::vector<PrimitiveInfo> primitiveInfo(primitiveSize);
        for (auto i{0}; i < primitiveSize; ++i) {
            primitiveInfo[i] = {i, mPrimitives[i]->BoundingBox()};
        }

        std::vector<Primitive *> orderedPrimitives;
        orderedPrimitives.reserve(primitiveSize);

        mRoot = RecursiveBuild(primitiveInfo, 0, primitiveSize, orderedPrimitives);
        mPrimitives.swap(orderedPrimitives);

        primitiveInfo.clear();
    }

    Boolean Intersect(const Ray &ray, SurfaceInteraction &interaction) const override {
        return RecursiveIntersect(mRoot, ray, interaction);
    }

private:

    BVHNode *RecursiveBuild(std::vector<PrimitiveInfo> &primitiveInfo,
                            Integer start, Integer end,
                            std::vector<Primitive *> &orderedPrimitives) {
        BVHNode *node = new BVHNode;
        Bound3f bound;
        for (auto i{start}; i < end; ++i) {
            bound = Union(bound, primitiveInfo[i].bounds);
        }

        Integer nPrimitive = end - start;
        if (nPrimitive <= 1) {
            // leaf node
            Integer firstPrimOffset = orderedPrimitives.size();
            for (auto i{start}; i < end; ++i) {
                Integer primNum = primitiveInfo[i].index;
                orderedPrimitives.push_back(mPrimitives[primNum]);
            }
            node->InitLeaf(firstPrimOffset, nPrimitive, bound);

            return node;
        } else {
            // interior node
            // split
            Bound3f centroidBound;
            for (auto i{start}; i < end; ++i) {
                centroidBound = Union(centroidBound, primitiveInfo[i].centroid);
            }
            Integer dim = MaxDimension(centroidBound);
            if (centroidBound.pMax[dim] == centroidBound.pMin[dim]) {
                // leaf node
                Integer firstPrimOffset = orderedPrimitives.size();
                for (auto i{start}; i < end; ++i) {
                    Integer primNum = primitiveInfo[i].index;
                    orderedPrimitives.push_back(mPrimitives[primNum]);
                }
                node->InitLeaf(firstPrimOffset, nPrimitive, bound);

                return node;
            } else {
                // interior node
                // default equal split
                Integer mid = (start + end) >> 1;
                std::nth_element(&primitiveInfo[start],
                                 &primitiveInfo[mid],
                                 &primitiveInfo[end - 1] + 1,
                                 [dim](const PrimitiveInfo &a, const PrimitiveInfo &b) {
                                     return a.centroid[dim] < b.centroid[dim];
                                 });
                node->InitInterior(RecursiveBuild(primitiveInfo, start, mid, orderedPrimitives),
                                   RecursiveBuild(primitiveInfo, mid, end, orderedPrimitives));
            }
        }
        return node;
    }

    Boolean
    RecursiveIntersect(const BVHNode *node, const Ray &ray, SurfaceInteraction &interaction) const {
        if (!node || !node->bound.IntersectP(ray, Infinity)) {
            return false;
        }

        // leaf node
        if (node->primitiveNumber > 0) {
            Boolean hitAnything{false};
            SurfaceInteraction tempInteraction;
            for (auto i{0}; i < node->primitiveNumber; ++i) {
                if (mPrimitives[node->firstPrimIndex + i]->Intersect(ray, tempInteraction)) {
                    hitAnything = true;

                    if (tempInteraction.t < interaction.t) {
                        interaction = tempInteraction;
                    }
                }
            }

            return hitAnything;
        }

        // interior node
        Boolean hitLeft = RecursiveIntersect(node->children[0], ray, interaction);
//            if (!hitLeft) {
////                std::print("{}\n", interaction.t);
//                interaction.t = Infinity;
//            }
        Boolean hitRight = RecursiveIntersect(node->children[1], ray, interaction);
        return hitLeft || hitRight;
    }

private:
    std::vector<Primitive *> mPrimitives;
    BVHNode *mRoot{nullptr};
};

REGISTER_CLASS(BVHAggregate, "bvh")

NAMESPACE_END
