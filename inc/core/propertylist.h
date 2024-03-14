//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/common.h"
#include "core/math.h"
#include <variant>
#include <string_view>
#include <unordered_map>
#include <functional>

NAMESPACE_BEGIN

class PropertyList {
public:

    void SetInt(std::string_view name, Int value);

    [[nodiscard]] Int GetInt(std::string_view name, Int defaultValue) const;

    void SetFloat(std::string_view name, Float value);

    [[nodiscard]] Float GetFloat(std::string_view name, Float defaultValue) const;

    void SetBool(std::string_view name, Bool value);

    [[nodiscard]] Bool GetBool(std::string_view name, Bool defaultValue) const;

    void SetString(std::string_view name, std::string_view value);

    [[nodiscard]] std::string_view GetString(std::string_view name, std::string_view defaultValue) const;

    void SetColor(std::string_view name, Color3f value);

    [[nodiscard]] Color3f GetColor(std::string_view name, Color3f defaultValue) const;

    void SetPoint(std::string_view name, Point3f value);

    [[nodiscard]] Point3f GetPoint(std::string_view name, Point3f defaultValue) const;

    void SetVector(std::string_view name, Vector3f value);

    [[nodiscard]] Vector3f GetVector(std::string_view name, Vector3f defaultValue) const;

    void SetTransform(std::string_view name, Transform value);

    [[nodiscard]] Transform GetTransform(std::string_view name, Transform defaultValue) const;

    void PrintDebugInfo() const;

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
    using Property = std::variant<Int, Float, Bool, std::string_view, Color3f, Point3f, Vector3f, Transform>;
    std::unordered_map<std::string_view, Property> mProperties;
};

NAMESPACE_END
