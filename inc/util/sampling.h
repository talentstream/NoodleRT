//
// Created by talentstream on 2024/2/24.
//

#pragma once

#include "core/math.h"

NAMESPACE_BEGIN

class Warp {
public:
    static Vector3f UniformSampleSphere(const Point2f& sample) {
        Float z = 1 - 2 * sample[0];
        Float r = SafeSqrt(1 - Sqr(z));
        Float phi = 2 * Pi * sample[1];
        return {r * Cos(phi), r * Sin(phi), z};
    }

    static Point2f UniformSampleTriangle(const Point2f& sample) {
        Float su0 = Sqrt(sample[0]);
        return {1 - su0, sample[1] * su0};
    }

    static Point2f ConcentricSampleDisk(Point2f sample) {
        Point2f p = 2 * sample - Vector2f{1, 1};
        if (p.x == 0 && p.y == 0) {
            return {0, 0};
        }
        Float theta, r;
        if (Abs(p.x) > Abs(p.y)) {
            r = p.x;
            theta = PiOver4 * (p.y / p.x);
        } else {
            r = p.y;
            theta = PiOver2 - PiOver4 * (p.x / p.y);
        }
        return {r * Cos(theta), r * Sin(theta)};
    }

    static Vector3f SquareToCosineHemisphere(Point2f sample2) {
        Point2f p = ConcentricSampleDisk(sample2);
        Float z = Sqrt(Max(0, 1 - Sqr(p.x) - Sqr(p.y)));

        return {p.x, p.y, z};
    }

    static Float SquareToCosineHemispherePdf(const Vector3f &v) {
        return Frame::AbsCosTheta(v) * InvPi;
    }
};
NAMESPACE_END
