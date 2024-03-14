//
// Created by 44772 on 2024/1/30.
//

#include "core/scene.h"
#include "base/integrator.h"
#include <print>

NAMESPACE_BEGIN

Scene::Scene(const PropertyList &propertyList) {
    mSpp = propertyList.GetInt("spp", 1);
    PRINT_DEBUG_INFO("Scene", "scene")
}

Scene::~Scene() {
    delete pIntegrator;
}

void Scene::AddChild(Object *object) {
    switch (object->GetClassType()) {
        case EClassType::EIntegrator:
            pIntegrator = dynamic_cast<Integrator *>(object);
            break;
        default:
            break;
    }
}



REGISTER_CLASS(Scene, "scene")

NAMESPACE_END