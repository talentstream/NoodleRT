//
// Created by 44772 on 2024/1/30.
//

#include "core/scene.h"
#include "base/camera.h"
#include "base/integrator.h"
#include "base/aggregate.h"
#include <print>

NAMESPACE_BEGIN

    Scene::Scene(const PropertyList &propertyList) {
        mSpp = propertyList.GetInteger("spp", 1);
        mMaxDepth = propertyList.GetInteger("depth", 1);
        mWidth = propertyList.GetInteger("width", 400);
        mHeight = propertyList.GetInteger("height", 400);
        PRINT_DEBUG_INFO("Scene", "scene")
    }

    Scene::~Scene() {
        delete pCamera;
        delete pIntegrator;
        delete pAggregate;
    }

    void Scene::AddChild(Object *object) {
        switch (object->GetClassType()) {
            case EClassType::ECamera:
                pCamera = static_cast<Camera *>(object);
                break;
            case EClassType::EIntegrator:
                pIntegrator = dynamic_cast<Integrator *>(object);
                break;
            case EClassType::EAggregate:
                pAggregate = static_cast<Aggregate *>(object);
                break;
            default:
                break;
        }
    }

    REGISTER_CLASS(Scene, "scene")

NAMESPACE_END