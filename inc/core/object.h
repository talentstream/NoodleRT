//
// Created by 44772 on 2024/1/27.
//

#pragma once

#include "core/common.h"
#include "propertylist.h"
#include <functional>
#include <memory>
#include <string_view>
#include <map>

NAMESPACE_BEGIN

    class Object {
    public:
        enum class EClassType {
            EScene = 0,
            EIntegrator,
            ECamera,
            EShape,
            EPrimitive,
            EAggregate,
            EMaterial
        };

        virtual ~Object() = default;

        [[nodiscard]] virtual EClassType GetClassType() const = 0;

        virtual void AddChild(Object *child) {/*throw*/}

        virtual void Initialize() {}

        static std::string ClassTypeName(EClassType type) {
            switch (type) {
                case EClassType::EScene:
                    return "EScene";
                case EClassType::EIntegrator:
                    return "EIntegrator";
                case EClassType::ECamera:
                    return "ECamera";
                case EClassType::EShape:
                    return "EShape";
                case EClassType::EPrimitive:
                    return "EPrimitive";
                case EClassType::EAggregate:
                    return "EAggregate";
                case EClassType::EMaterial:
                    return "EMaterial";
                default:
                    return "Unknown";
            };
        }

    };

    class ObjectFactory {
    public:
        using Creator = std::function<Object *(const PropertyList &)>;

        static void RegisterClass(std::string_view name, const Creator &creator);

        static Object *CreateInstance(std::string_view name, const PropertyList &propertyList);


    private:
        static constinit std::unordered_map<std::string_view, Creator> *pCreators;
    };

#define REGISTER_CLASS(Class, Name) \
Class *Class ##Create(const PropertyList& propertyList) { return new Class(propertyList); } \
static struct Class ##Register { \
    Class ##Register() { \
        ObjectFactory::RegisterClass(Name, Class ##Create); \
    } \
} Class ##RegisterInstance;

#define PRINT_DEBUG_INFO(Name,Type) \
std::print("{}->{} --- ",Name,Type); \
propertyList.PrintDebugInfo(); \
std::print("\n");

NAMESPACE_END