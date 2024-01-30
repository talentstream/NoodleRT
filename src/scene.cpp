//
// Created by 44772 on 2024/1/30.
//

#include "core/scene.h"
#include <print>

NAMESPACE_BEGIN

    Scene::Scene(const PropertyList &) {
        std::print("->Scene\n");
    }

    Scene::~Scene() {
        delete pCamera;
        delete pIntegrator;
        delete pAggregate;
    }

    REGISTER_CLASS(Scene, "scene")

NAMESPACE_END