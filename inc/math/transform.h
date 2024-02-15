//
// Created by 44772 on 2024/2/15.
//

#pragma once

#include "core/common.h"
#include "math/constant.h"

NAMESPACE_BEGIN

struct Matrix4x4 {
    Float m[4][4];

    Matrix4x4() {
        m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
        m[0][1] = m[0][2] = m[0][3] = 0;
        m[1][0] = m[1][2] = m[1][3] = 0;
        m[2][0] = m[2][1] = m[2][3] = 0;
    }

    Matrix4x4(Float other[4][4]) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++) {
                m[i][j] = other[i][j];
            }
    }

    Matrix4x4(Float m00, Float m01, Float m02, Float m03,
              Float m10, Float m11, Float m12, Float m13,
              Float m20, Float m21, Float m22, Float m23,
              Float m30, Float m31, Float m32, Float m33)
            : m{{m00, m01, m02, m03},
                {m10, m11, m12, m13},
                {m20, m21, m22, m23},
                {m30, m31, m32, m33}} {}

    Float *operator[](int i) {
        return m[i];
    }

    const Float *operator[](int i) const {
        return m[i];
    }

    Boolean operator==(const Matrix4x4 &other) const {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j) {
                if (m[i][j] != other.m[i][j]) {
                    return false;
                }
            }
        return true;
    }

    Boolean operator!=(const Matrix4x4 &other) const {
        return !(*this == other);
    }

    friend Matrix4x4 Transpose(const Matrix4x4 &other) {
        return {other.m[0][0], other.m[1][0], other.m[2][0], other.m[3][0],
                other.m[0][1], other.m[1][1], other.m[2][1], other.m[3][1],
                other.m[0][2], other.m[1][2], other.m[2][2], other.m[3][2],
                other.m[0][3], other.m[1][3], other.m[2][3], other.m[3][3]};
    }

    static Matrix4x4 Mul(const Matrix4x4 &other1, const Matrix4x4 &other2) {
        Matrix4x4 r;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j) {
                r.m[i][j] = other1.m[i][0] * other2.m[0][j] +
                            other1.m[i][1] * other2.m[1][j] +
                            other1.m[i][2] * other2.m[2][j] +
                            other1.m[i][3] * other2.m[3][j];
            }
        return r;
    }

    // https://www.geeksforgeeks.org/cpp-program-for-determinant-of-a-matrix/
    friend Float Determinant(const Matrix4x4 &other) {
        Float det{1};
        auto mat = other;
        for (int i = 0; i < 4; ++i) {
            int pivot = i;
            for (int j = i + 1; j < 4; ++j) {
                if (Abs(mat[i][j]) > Abs(mat[pivot][j])) {
                    pivot = j;
                }
            }
            if (pivot != i) {
                for (int j = 0; j < 4; ++j) {
                    Swap(mat[j][i], mat[j][pivot]);
                }
                det *= -1;
            }
            if (mat[i][i] == 0) {
                return 0;
            }
            det *= mat[i][i];
            for (int j = i + 1; j < 4; ++j) {
                Float ratio = mat[j][i] / mat[i][i];
                for (int k = 0; k < 4; ++k) {
                    mat[j][k] = mat[j][j] - ratio * mat[i][k];
                }
            }
        }
        return det;
    }

    friend Matrix4x4 Inverse(const Matrix4x4 &other) {
        Float determinant[4]{};
    }
};

NAMESPACE_END