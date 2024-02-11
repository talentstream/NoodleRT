//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/object.h"

NAMESPACE_BEGIN

class Aggregate;

class Camera;

class Integrator : public Object {
public:
    virtual ~Integrator() = default;

    virtual std::vector<Color3f> Render() const = 0;

    void AddChild(Object *child) override;

    void Initialize() override;

    [[nodiscard]] EClassType GetClassType() const override {
        return EClassType::EIntegrator;
    }

    const Camera *GetCamera() const {
        return pCamera;
    }

protected:
    Aggregate *pAggregate{nullptr};
    Camera *pCamera{nullptr};
    // std::vector<Light> mLights;
    // Sampler *pSampler{nullptr};
};

class ImageTileIntegrator : public Integrator {
public:
    ~ImageTileIntegrator() = default;

    std::vector<Color3f> Render() const final;

    [[nodiscard]] virtual Color3f Li(const Ray &ray) const = 0;
};

NAMESPACE_END
