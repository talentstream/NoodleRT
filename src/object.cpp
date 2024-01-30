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

Object* ObjectFactory::CreateInstance(std::string_view name, const PropertyList& propertyList) {
    if(pCreators == nullptr || !pCreators->contains(name)) {
        throw std::runtime_error("ObjectFactory::CreateInstance: class name not found");
    }
    return (*pCreators)[name](propertyList);
}

NAMESPACE_END
