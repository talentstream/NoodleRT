//
// Created by talentstream on 2024/1/20.
//

#pragma once

#include "core/common.h"
#include "math/vec.h"
#include "math/color.h"
#include <random>

NAMESPACE_BEGIN


static const Float FloatOneMinusEpsilon = 0.99999994;
static const Float OneMinusEpsilon = FloatOneMinusEpsilon;

#define PCG32_DEFAULT_STATE 0x853c49e6748fea9bULL
#define PCG32_DEFAULT_STREAM 0xda3e39cb94b95bdbULL
#define PCG32_MULT 0x5851f42d4c957f2dULL


class RNG {
public:
    RNG() : state(PCG32_DEFAULT_STATE), inc(PCG32_DEFAULT_STREAM) {}

    RNG(uint64_t sequenceIndex) { SetSequence(sequenceIndex); }

    Float UniformFloat() {
        auto result = std::min(OneMinusEpsilon, Float(UniformUInt32() * 2.3283064365386963e-10f));
        return result;
    }

private:
    void SetSequence(uint64_t sequenceIndex) {
        state = 0U;
        inc = (sequenceIndex << 1u) | 1u;
        UniformUInt32();
        state += PCG32_DEFAULT_STATE;
        UniformUInt32();
    }

    uint32_t UniformUInt32() {
        uint64_t oldstate = state;
        state = oldstate * PCG32_MULT + inc;
        uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
        uint32_t rot = oldstate >> 59u;
        return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
    }

    uint64_t state;
    uint64_t inc;
};

static RNG GlobalRNG;

inline Float RandomFloat() {
    return GlobalRNG.UniformFloat();
}

inline Float RandomFloat(Float min, Float max) {
    return min + (max - min) * RandomFloat();
}

inline Vector3f RandomVector3f() {
    return Vector3f{RandomFloat(), RandomFloat(), RandomFloat()};
}

inline Vector3f RandomVector3f(Float min, Float max) {
    return Vector3f{RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max)};
}

inline Point3f RandomPoint3f() {
    return Point3f{RandomFloat(), RandomFloat(), RandomFloat()};
}

inline Point3f RandomPoint3f(Float min, Float max) {
    return Point3f{RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max)};
}

inline Color3f RandomColor3f() {
    return Color3f{RandomFloat(), RandomFloat(), RandomFloat()};
}

inline Color3f RandomColor3f(Float min, Float max) {
    return Color3f{RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max)};
}

inline Vector3f RandomInUnitSphere() {
    while (true) {
        auto p = RandomVector3f(-1, 1);
        if (LengthSquared(p) >= 1)
            continue;
        return p;
    }
}

inline Vector3f RandomInUnitVector() {
    return Normalize(RandomInUnitSphere());
}

inline Vector3f RandomInHemisphere(Normal3f normal) {
    auto inUnitSphere = RandomInUnitSphere();
    if (Dot(inUnitSphere, Vector3f{normal}) > 0) {
        return inUnitSphere;
    } else {
        return -inUnitSphere;
    }
}


NAMESPACE_END
