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
        m[3][0] = m[3][1] = m[3][2] = 0;
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
                    mat[j][k] -= ratio * mat[i][k];
                }
            }
        }
        return det;
    }

    friend Matrix4x4 Inverse(const Matrix4x4 &other) {
        Matrix4x4 inv;

        Float augmented[4][8];
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                augmented[i][j] = other[i][j];
                augmented[i][j + 4] = i == j ? 1 : 0;
            }
        }

        for (int i = 0; i < 4; ++i) {
            int pivot = i;
            for (int j = i + 1; j < 4; ++j) {
                if (Abs(augmented[j][i]) > Abs(augmented[pivot][i])) {
                    pivot = j;
                }
            }
            if (pivot != i) {
                for (int j = 0; j < 8; ++j) {
                    Swap(augmented[i][j], augmented[pivot][j]);
                }
            }
            if (augmented[i][i] == 0) {
                return {};
            }
            for (int j = 0; j < 8; ++j) {
                augmented[i][j] /= augmented[i][i];
            }
            for (int j = 0; j < 4; ++j) {
                if (j == i) {
                    continue;
                }
                Float ratio = augmented[j][i];
                for (int k = 0; k < 8; ++k) {
                    augmented[j][k] -= ratio * augmented[i][k];
                }
            }
        }

        // Divide each row by its pivot
        for (int i = 0; i < 4; ++i) {
            Float pivot = augmented[i][i];
            for (int j = 0; j < 8; ++j) {
                augmented[i][j] /= pivot;
            }
        }

        // Extract inverse matrix from augmented matrix
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                inv[i][j] = augmented[i][j + 4];
            }
        }

        return inv;
    }
};

inline Matrix4x4 operator*(const Matrix4x4 &other1, const Matrix4x4 &other2) {
    return Matrix4x4::Mul(other2, other1);
}

class Transform {
public:
    Transform() = default;

    explicit Transform(const Matrix4x4 &m) : mat(m), invMat(Inverse(m)) {}

    explicit Transform(const Float m[4][4]) {
        mat = Matrix4x4(m[0][0], m[0][1], m[0][2], m[0][3],
                        m[1][0], m[1][1], m[1][2], m[1][3],
                        m[2][0], m[2][1], m[2][2], m[2][3],
                        m[3][0], m[3][1], m[3][2], m[3][3]);
        invMat = Inverse(mat);
    }

    template<typename T>
    Point3<T> operator()(const Point3<T> &p) const {
        T x = p.x, y = p.y, z = p.z;
        T xp = mat[0][0] * x + mat[0][1] * y + mat[0][2] * z + mat[0][3];
        T yp = mat[1][0] * x + mat[1][1] * y + mat[1][2] * z + mat[1][3];
        T zp = mat[2][0] * x + mat[2][1] * y + mat[2][2] * z + mat[2][3];
        T wp = mat[3][0] * x + mat[3][1] * y + mat[3][2] * z + mat[3][3];
        if (wp == 1) {
            return {xp, yp, zp};
        } else {
            return Point3<T>{xp, yp, zp} / wp;
        }
    }

    template<typename T>
    Vector3<T> operator()(const Vector3<T> &v) const {
        T x = v.x, y = v.y, z = v.z;
        T xv = mat[0][0] * x + mat[0][1] * y + mat[0][2] * z + mat[0][3];
        T yv = mat[1][0] * x + mat[1][1] * y + mat[1][2] * z + mat[1][3];
        T zv = mat[2][0] * x + mat[2][1] * y + mat[2][2] * z + mat[2][3];
        T wv = mat[3][0] * x + mat[3][1] * y + mat[3][2] * z + mat[3][3];
        if (wv == 1) {
            return {xv, yv, zv};
        } else {
            return Vector3<T>{xv, yv, zv} / wv;
        }
    }

    Transform operator*(const Transform &other) const {
        return Transform(Matrix4x4::Mul(mat, other.mat));
    }

    Transform operator*=(const Transform &other) {
        mat = Matrix4x4::Mul(mat, other.mat);
        invMat = Inverse(mat);
        return *this;
    }

    static Transform Scale(Float x, Float y, Float z) {
        return Transform(Matrix4x4(x, 0, 0, 0,
                                   0, y, 0, 0,
                                   0, 0, z, 0,
                                   0, 0, 0, 1));
    }

    static Transform Translate(Float x, Float y, Float z) {
        return Transform(Matrix4x4(1, 0, 0, x,
                                   0, 1, 0, y,
                                   0, 0, 1, z,
                                   0, 0, 0, 1));
    }

    static Transform Orthographic(Float n, Float f) {
        return Scale(1.f, 1.f, 1.f / (f - n)) *
               Translate(0.f, 0.f, -n);
    }

    static Transform Perspective(Float fov, Float n, Float f) {
        Float recip = 1.f / (f - n);
        Float cot = 1.f / Tan(DegreeToRadian(fov * .5f));
        Matrix4x4 perspective(cot, 0, 0, 0,
                              0, cot, 0, 0,
                              0, 0, f * recip, -f * n * recip,
                              0, 0, 1, 0);
        return Transform(perspective);
    }

    void inverse() {
        std::swap(mat, invMat);
    }

    void Print() const {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                std::cout << mat[i][j] << " ";
            }
        }
    }

private:
    Matrix4x4 mat;
    Matrix4x4 invMat;
};

NAMESPACE_END