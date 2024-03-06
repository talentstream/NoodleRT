//
// Created by 44772 on 2024/1/27.
//

#include "core/object.h"

#include <stdexcept>

NAMESPACE_BEGIN

constinit std::unordered_map<std::string_view, ObjectFactory::Creator> *ObjectFactory::pCreators = nullptr;

void ObjectFactory::RegisterClass(std::string_view name, const ObjectFactory::Creator &creator) {
    if (pCreators == nullptr) {
        pCreators = new std::unordered_map<std::string_view, Creator>();
    }
    pCreators->emplace(name, creator);
}

Object *ObjectFactory::CreateInstance(std::string_view name, const PropertyList &propertyList, Boolean manualInit) {
    if (pCreators == nullptr || !pCreators->contains(name)) {
        throw std::runtime_error(std::format("ObjectFactory::CreateInstance: class name {} not found", name));
    }
    auto object = (*pCreators)[name](propertyList);
    if (manualInit) {
        object->Initialize();
    }
    return object;
}

NAMESPACE_END

