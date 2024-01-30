//
// Created by 44772 on 2024/1/30.
//

#include "core/propertylist.h"

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

NAMESPACE_END

