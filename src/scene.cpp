//
// Created by 44772 on 2024/1/30.
//

#include "core/scene.h"

NAMESPACE_BEGIN

Scene::~Scene() {
    delete pCamera;
    delete pIntegrator;
    delete pAggregate;
}

NAMESPACE_END