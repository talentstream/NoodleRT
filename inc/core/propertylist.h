//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/common.h"

#include <variant>
#include <string_view>
#include <unordered_map>
#include <functional>

NAMESPACE_BEGIN

    class PropertyList {
    public:

        void SetInteger(std::string_view name, Integer value);

        [[nodiscard]] Integer GetInteger(std::string_view name, Integer defaultValue) const;

        void SetFloat(std::string_view name, Float value);

        [[nodiscard]] Float GetFloat(std::string_view name, Float defaultValue) const;

        void SetBoolean(std::string_view name, Boolean value);

        [[nodiscard]] Boolean GetBoolean(std::string_view name, Boolean defaultValue) const;

        void SetString(std::string_view name, std::string_view value);

        [[nodiscard]] std::string_view GetString(std::string_view name, std::string_view defaultValue) const;

    private:
        template<typename T>
        void Set(std::string_view name, T value) {
            mProperties[name] = value;
        }

        template<typename T>
        T Get(std::string_view name, T defaultValue) const {
            if (!mProperties.contains(name)) {
                return defaultValue;
            }
            return std::get<T>(mProperties.at(name));
        }


    private:
        using Property = std::variant<Integer, Float, Boolean,std::string_view>;
        std::unordered_map<std::string_view, Property> mProperties;
    };

NAMESPACE_END
