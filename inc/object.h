//
// Created by 44772 on 2024/1/27.
//

#pragma once

#include <functional>
#include <memory>
#include <string_view>
#include <map>

class Object {
public:
    enum class EClassType {
        EScene = 0,
        EIntegrator,
        ECamera
    };

    virtual ~Object() = default;

    [[nodiscard]] virtual EClassType GetClassType() const = 0;

    static std::string ClassTypeName(EClassType type) {
        switch (type) {
            case EClassType::EScene:
                return "EScene";
            case EClassType::EIntegrator:
                return "EIntegrator";
            case EClassType::ECamera:
                return "ECamera";
            default:
                return "Unknown";
        };
    }

};

class ObjectFactory {
public:
    using Creator = std::function<Object *()>;

    static void RegisterClass(std::string_view name, const Creator &creator);

    static Object *CreateInstance(std::string_view name);


private:
    static constinit std::unordered_map<std::string_view, Creator> *pCreators;
};

#define REGISTER_CLASS(Class, Name) \
    Class *Class ##Create() { return new Class(); } \
    static struct Class ##Register { \
        Class ##Register() { \
            ObjectFactory::RegisterClass(Name, Class ##Create); \
        } \
    } Class ##RegisterInstance;