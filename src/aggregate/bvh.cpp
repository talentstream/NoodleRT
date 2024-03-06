//
// Created by 44772 on 2024/2/3.
//

#include "base/aggregate.h"

#include <stack>
#include <vector>
#include <algorithm>

NAMESPACE_BEGIN

struct BVHNode {
    Bound3f bbox;
    // for leaf
    UInt32 startIdx{0};// start index in mShapeIndices
    UInt32 size{0};// start + size = end index in mShapeIndices

    // for inner
    UInt32 rightChildIdx{0};// right child index in mNodes

    void InitLeaf(UInt32 s, UInt32 n) {
        startIdx = s;
        size = n;
    }

    void InitInner(UInt32 r) {
        rightChildIdx = r;
    }

    Boolean IsLeaf() const {
        return size > 0;
    }

    Boolean IsInner() const {
        return size == 0;
    }

    Boolean IsUnused() const {
        return size == 0 && rightChildIdx == 0;
    }
};

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
        mCloneIndices = mShapeIndices;
        RecursiveBuild(0, 0, size);
    }

    Boolean
    Intersect(const Ray &ray, IntersectionRecord &iRec) const override {
        if (mNodes.empty()) return false;

        Boolean foundIntersection{false};
        UInt32 nodeIdx = 0;
        std::stack<UInt32> nodeIdxStack;
        while (true) {
            const auto &node = mNodes[nodeIdx];

            if (node.IsUnused() || !node.bbox.IntersectP(ray, Infinity)) {
                if (nodeIdxStack.empty()) {
                    break;
                }
                nodeIdx = nodeIdxStack.top();
                nodeIdxStack.pop();
                continue;
            }

            if (node.IsInner()) {
                nodeIdxStack.push(node.rightChildIdx);
                nodeIdx++;
            } else {
                for (auto i{node.startIdx}; i < node.startIdx + node.size; i++) {

                    auto idx = mShapeIndices[i];

                    const auto s = mShapes[FindShape(idx)];
                    if (s->Intersect(idx, ray, iRec)) {
                        foundIntersection = true;
                    }
                }

                if (nodeIdxStack.empty()) {
                    break;
                }
                nodeIdx = nodeIdxStack.top();
                nodeIdxStack.pop();
            }
        }

        return foundIntersection;
    }

    Boolean
    UnOccluded(const Ray &ray) const override {
        UInt32 nodeIdx = 0;
        std::stack<UInt32> nodeIdxStack;
        while (true) {
            const auto &node = mNodes[nodeIdx];

            if (node.IsUnused() || !node.bbox.IntersectP(ray, Infinity)) {
                if (nodeIdxStack.empty()) {
                    break;
                }
                nodeIdx = nodeIdxStack.top();
                nodeIdxStack.pop();
                continue;
            }

            if (node.IsInner()) {
                nodeIdxStack.push(node.rightChildIdx);
                nodeIdx++;
            } else {
                for (auto i{node.startIdx}; i < node.startIdx + node.size; i++) {
                    auto idx = mShapeIndices[i];
                    const auto s = mShapes[FindShape(idx)];
                    if (s->IntersectP(idx, ray)) {
                        return false;
                    }
                }

                if (nodeIdxStack.empty()) {
                    break;
                }
                nodeIdx = nodeIdxStack.top();
                nodeIdxStack.pop();
            }
        }
        return true;
    }

private:
    // nodeIdx for mNodes
    // start, end for mShapeIndices
    void RecursiveBuild(UInt32 nodeIdx, UInt32 start, UInt32 end) {
        BVHNode &node = mNodes[nodeIdx];
        auto size = end - start;
        if (size == 0) return;

        // set node bbox
        Bound3f bound;
        for (auto i{start}; i < end; i++) {
            bound.Expand(FindShapeBoundingBox(i));
        }
        node.bbox = bound;

        // Middle split
        if (size == 1) {
            node.InitLeaf(start, 1);
            return;
        }

        Bound3f centroidBound;
        for (auto i{start}; i < end; i++) {
            centroidBound.Expand(FindShapeCentroid(i));
        }
        auto dim = MaxDimension(centroidBound);
        if (centroidBound.pMax[dim] == centroidBound.pMin[dim]) {
            node.InitLeaf(start, size);
            return;
        }

        auto mid = (start + end) >> 1;
        std::nth_element(&mCloneIndices[start],
                         &mCloneIndices[mid],
                         &mCloneIndices[end - 1] + 1,
                         [dim, this](UInt32 a, UInt32 b) {
                             return FindShapeCentroid(a)[dim] < FindShapeCentroid(b)[dim];
                         });

        auto leftCount = mid - start;
        auto nodeIdxLeft = nodeIdx + 1;
        auto nodeIdxRight = nodeIdx + 2 * leftCount;
        node.InitInner(nodeIdxRight);
        RecursiveBuild(nodeIdxLeft, start, mid);
        RecursiveBuild(nodeIdxRight, mid, end);
    }

private:
    std::vector<UInt32> mCloneIndices;
    std::vector<BVHNode> mNodes;
};

REGISTER_CLASS(BVH, "bvh")

NAMESPACE_END