//
// Created by 44772 on 2024/1/30.
//

#include "core/propertylist.h"
#include <print>

NAMESPACE_BEGIN

#define PROPERTY_ACCESSOR(Name, Type) \
    Type PropertyList::Get##Name(std::string_view name,Type defaultValue) const { \
        if(!mProperties.contains(name)){\
            return defaultValue;\
        }              \
        return std::get<Type>(mProperties.at(name)); \
    } \
    void PropertyList::Set##Name(std::string_view name, Type value) { \
        mProperties[name] = value; \
    }

PROPERTY_ACCESSOR(Integer, Integer)

PROPERTY_ACCESSOR(Float, Float)

PROPERTY_ACCESSOR(Boolean, Boolean)

PROPERTY_ACCESSOR(String, std::string_view)

PROPERTY_ACCESSOR(Color, Color3f)

PROPERTY_ACCESSOR(Point, Point3f)

PROPERTY_ACCESSOR(Vector, Vector3f)

PROPERTY_ACCESSOR(Transform, Transform)

void PropertyList::PrintDebugInfo() const {
    for (const auto &[name, value]: mProperties) {
        std::print("{}: ", name);
        std::visit([](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, Integer>) {
                std::print("{} ", arg);
            } else if constexpr (std::is_same_v<T, Float>) {
                std::print("{} ", arg);
            } else if constexpr (std::is_same_v<T, Boolean>) {
                std::print("{} ", arg);
            } else if constexpr (std::is_same_v<T, std::string_view>) {
                std::print("{} ", arg);
            } else if constexpr (std::is_same_v<T, Color3f>) {
                std::print("{} {} {} ", arg.r, arg.g, arg.b);
            } else if constexpr (std::is_same_v<T, Point3f>) {
                std::print("{} {} {} ", arg.x, arg.y, arg.z);
            } else if constexpr (std::is_same_v<T, Vector3f>) {
                std::print("{} {} {} ", arg.x, arg.y, arg.z);
            } else if constexpr (std::is_same_v<T, Transform>) {
                arg.Print();
            } else {
                /*throw*/
            }
        }, value);
    }
}

NAMESPACE_END

