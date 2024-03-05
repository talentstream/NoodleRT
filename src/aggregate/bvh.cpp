//
// Created by 44772 on 2024/2/3.
//

#include "base/aggregate.h"

#include <stack>
#include <vector>
#include <algorithm>

NAMESPACE_BEGIN

class BVH : public Aggregate {
public:
    explicit BVH(const PropertyList &propertyList) {
        mShapeOffset.push_back(0u);
        PRINT_DEBUG_INFO("aggregate", "bvh")
    }

    void
    Build() override {
        auto size = GetPrimitiveCount();
        if (size == 0) return;
        mNodes.resize(2 * size);
        mNodes[0].bbox = mBbox;
        mShapeIndices.resize(size);
        for (auto i{0}; i < size; i++) {
            mShapeIndices[i] = i;
        }
        std::vector<UInt32> ordered;
        ordered.reserve(size);
        RecursiveBuild(0, 0, size, ordered);
        mShapeIndices.swap(ordered);
    }

    Boolean
    Intersect(const Ray &ray, IntersectionRecord &si) const override {
        Boolean hitAnything{false};

        UInt32 nodeIdx = 0;
        std::stack<UInt32> nodeIdxStack;

        if (mNodes.empty()) {
            return false;
        }
        IntersectionRecord tempIRec;
        while (true) {
            const auto &node = mNodes[nodeIdx];
            if (!node.bbox.IntersectP(ray, si.t)) {
                if (nodeIdxStack.empty()) break;
                nodeIdx = nodeIdxStack.top();
                nodeIdxStack.pop();
                continue;
            }

            if (node.IsInner()) {
                nodeIdxStack.push(node.rightChild);
            } else {
                for (auto i{node.Start()}, end{node.End()}; i < end; i++) {
                    auto idx = mShapeIndices[i];
                    const auto s = mShapes[FindShape(idx)];
                    if (s->Intersect(idx, ray, si)) {
                        hitAnything = true;
                        if (tempIRec.t < si.t) {
                            si = tempIRec;
                        }
                    }
                }
            }
            if (nodeIdxStack.empty()) break;
            nodeIdx = nodeIdxStack.top();
            nodeIdxStack.pop();
        }

        return hitAnything;
    }

    Boolean
    UnOccluded(const Ray &ray) const override {
        return true;
    }

private:
    void RecursiveBuild(UInt32 nodeIdx, UInt32 start, UInt32 end, std::vector<UInt32> &ordered) {

    }

private:
    struct BVHNode {
        Bound3f bbox;

        // leaf
        UInt32 beginIdx;// shape begin idx
        UInt32 size;// shape size

        // inner
        UInt32 rightChild;

        enum class NodeType {
            Leaf,
            Inner
        } type;

        void InitLeaf(UInt32 idx, UInt32 n) {
            beginIdx = idx;
            size = n;
            type = NodeType::Leaf;
        }

        void InitInner(UInt32 right) {
            rightChild = right;
            type = NodeType::Inner;
        }

        Boolean IsLeaf() const {
            return type == NodeType::Leaf;
        }

        Boolean IsInner() const {
            return type == NodeType::Inner;
        }

        UInt32 Start() const {
            return beginIdx;
        }

        UInt32 End() const {
            return beginIdx + size;
        }

    };

    std::vector<BVHNode> mNodes;
};

REGISTER_CLASS(BVH, "bvh")

NAMESPACE_END
//
//struct PrimitiveInfo {
//    PrimitiveInfo() = default;
//
//    PrimitiveInfo(Integer i, Bound3f b)
//            : index{i}, bounds{b},
//              centroid{0.5f * b.pMin + 0.5f * b.pMax} {}
//
//    Integer index;
//    Bound3f bounds;
//    Point3f centroid;
//};
//
//
//struct BVHNode {
//    void InitLeaf(Integer first, Integer n, Bound3f b) {
//        firstPrimIndex = first;
//        primitiveNumber = n;
//        bound = b;
//        children[0] = children[1] = nullptr;
//    }
//
//    void InitInterior(BVHNode *c0, BVHNode *c1) {
//        children[0] = c0;
//        children[1] = c1;
//        bound = Union(c0->bound, c1->bound);
//    }
//
//    Bound3f bound;
//    BVHNode *children[2];
//    Integer firstPrimIndex;
//    Integer primitiveNumber{0};
//};
//
//
//// 1. bvh construction
////    1.1 compute bounding information
////    1.2 select building info
////    1.3 build bvh
//// 2. bvh traversal
//class BVHAggregate : public aggregate1 {
//public:
//    explicit BVHAggregate(const PropertyList &propertyList) {
//        PRINT_DEBUG_INFO("Aggregate", "bvh")
//    }
//
//    void AddChild(Object *child) override {
//        switch (child->GetClassType()) {
//            case EClassType::EPrimitive:
//                mPrimitives.emplace_back(dynamic_cast<Primitive *>(child));
//                break;
//            case EClassType::EMesh: {
////                auto mesh = dynamic_cast<Mesh *>(child);
////                mPrimitives.insert(mPrimitives.end(), mesh->primitives.begin(), mesh->primitives.end());
////                break;
//            }
//            default:
//                /*throw*/
//                break;
//        }
//    }
//
//    void Initialize() override {
//        if (mPrimitives.empty()) return;
//        Integer primitiveSize = mPrimitives.size();
//        std::vector<PrimitiveInfo> primitiveInfo(primitiveSize);
//        for (auto i{0}; i < primitiveSize; ++i) {
//            primitiveInfo[i] = {i, mPrimitives[i]->BoundingBox()};
//        }
//
//        std::vector<Primitive *> orderedPrimitives;
//        orderedPrimitives.reserve(primitiveSize);
//
//        mRoot = RecursiveBuild(primitiveInfo, 0, primitiveSize, orderedPrimitives);
//        mPrimitives.swap(orderedPrimitives);
//
//        primitiveInfo.clear();
//    }
//
//    Boolean
//    Intersect(const Ray &ray, IntersectionRecord &interaction) const override {
//        return RecursiveIntersect(mRoot, ray, interaction);
//    }
//
//    Boolean
//    UnOccluded(const Ray &ray) const override {
//        return RecursiveIntersectP(mRoot, ray);
//    }
//
//private:
//
//    BVHNode *RecursiveBuild(std::vector<PrimitiveInfo> &primitiveInfo,
//                            Integer start, Integer end,
//                            std::vector<Primitive *> &orderedPrimitives) {
//        BVHNode *node = new BVHNode;
//        Bound3f bound;
//        for (auto i{start}; i < end; ++i) {
//            bound = Union(bound, primitiveInfo[i].bounds);
//        }
//
//        Integer nPrimitive = end - start;
//        if (nPrimitive <= 1) {
//            // leaf node
//            Integer firstPrimOffset = orderedPrimitives.size();
//            for (auto i{start}; i < end; ++i) {
//                Integer primNum = primitiveInfo[i].index;
//                orderedPrimitives.push_back(mPrimitives[primNum]);
//            }
//            node->InitLeaf(firstPrimOffset, nPrimitive, bound);
//
//            return node;
//        } else {
//            // interior node
//            // split
//            Bound3f centroidBound;
//            for (auto i{start}; i < end; ++i) {
//                centroidBound = Union(centroidBound, primitiveInfo[i].centroid);
//            }
//            Integer dim = MaxDimension(centroidBound);
//            if (centroidBound.pMax[dim] == centroidBound.pMin[dim]) {
//                // leaf node
//                Integer firstPrimOffset = orderedPrimitives.size();
//                for (auto i{start}; i < end; ++i) {
//                    Integer primNum = primitiveInfo[i].index;
//                    orderedPrimitives.push_back(mPrimitives[primNum]);
//                }
//                node->InitLeaf(firstPrimOffset, nPrimitive, bound);
//
//                return node;
//            } else {
//                // interior node
//                // default equal split
//                Integer mid = (start + end) >> 1;
//                std::nth_element(&primitiveInfo[start],
//                                 &primitiveInfo[mid],
//                                 &primitiveInfo[end - 1] + 1,
//                                 [dim](const PrimitiveInfo &a, const PrimitiveInfo &b) {
//                                     return a.centroid[dim] < b.centroid[dim];
//                                 });
//                node->InitInterior(RecursiveBuild(primitiveInfo, start, mid, orderedPrimitives),
//                                   RecursiveBuild(primitiveInfo, mid, end, orderedPrimitives));
//            }
//        }
//        return node;
//    }
//
//    Boolean
//    RecursiveIntersect(const BVHNode *node, const Ray &ray, IntersectionRecord &interaction) const {
//        if (!node || !node->bound.IntersectP(ray, Infinity)) {
//            return false;
//        }
//
//        // leaf node
//        if (node->primitiveNumber > 0) {
//            Boolean hitAnything{false};
//            IntersectionRecord tempInteraction;
//            for (auto i{0}; i < node->primitiveNumber; ++i) {
//                if (mPrimitives[node->firstPrimIndex + i]->Intersect(ray, tempInteraction)) {
//                    hitAnything = true;
//
//                    if (tempInteraction.t < interaction.t) {
//                        interaction = tempInteraction;
//                    }
//                }
//            }
//
//            return hitAnything;
//        }
//
//        // interior node
//        Boolean hitLeft = RecursiveIntersect(node->children[0], ray, interaction);
////            if (!hitLeft) {
//////                std::print("{}\n", interaction.t);
////                interaction.t = Infinity;
////            }
//        Boolean hitRight = RecursiveIntersect(node->children[1], ray, interaction);
//        return hitLeft || hitRight;
//    }
//
//    Boolean
//    RecursiveIntersectP(const BVHNode *node, const Ray &ray) const {
//        if (!node || !node->bound.IntersectP(ray, Infinity)) {
//            return false;
//        }
//
//        // leaf node
//        if (node->primitiveNumber > 0) {
//            for (auto i{0}; i < node->primitiveNumber; ++i) {
//                if (mPrimitives[node->firstPrimIndex + i]->IntersectP(ray)) {
//                    return true;
//                }
//            }
//            return false;
//        }
//
//        // interior node
//        return RecursiveIntersectP(node->children[0], ray) || RecursiveIntersectP(node->children[1], ray);
//    }
//
//private:
//    std::vector<Primitive *> mPrimitives;
//    BVHNode *mRoot{nullptr};
//};
//
//REGISTER_CLASS(BVHAggregate, "bvh")
//
//NAMESPACE_END
