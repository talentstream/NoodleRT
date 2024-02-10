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
            EMesh,
            EBxDF,
            ETexture,
            ESampler
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
                case EClassType::EMesh:
                    return "EMesh";
                case EClassType::EBxDF:
                    return "EBxDF";
                case EClassType::ETexture:
                    return "ETexture";
                case EClassType::ESampler:
                    return "ESampler";
                default:
                    return "Unknown";
            };
        }

    };

    class ObjectFactory {
    public:
        using Creator = std::function<Object *(const PropertyList &)>;

        static void RegisterClass(std::string_view name, const Creator &creator);

        static Object *CreateInstance(std::string_view name, const PropertyList &propertyList, Boolean manualInit = false);


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

#ifdef NDEBUG
#define PRINT_DEBUG_INFO(Name,Type)
#else
    #define PRINT_DEBUG_INFO(Name,Type) \
std::print("{}->{} --- ",Name,Type); \
propertyList.PrintDebugInfo();          \
std::print("\n");
#endif
NAMESPACE_END