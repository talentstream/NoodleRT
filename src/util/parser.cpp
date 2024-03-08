// Created by 44772 on 2024/1/30.
//

#include "util/parser.h"
#include "core/propertylist.h"
#include "core/math.h"
#include "pugixml.hpp"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stdexcept>
#include <format>
#include <print>
#include <ranges>

NAMESPACE_BEGIN

using std::string_view;

// Todo: more string check
Boolean ToBoolean(string_view str) {
    return str == "true";
}

Integer ToInteger(string_view str) {
    return std::stoi(str.data());
}

Float ToFloat(string_view str) {
    return std::stof(str.data());
}

Vector3f ToVector(string_view str) {
    auto tokens = str | std::views::split(' ');
    Vector3f result{};
    for (const auto [i, token]: tokens | std::views::enumerate) {
        result[i] = ToFloat(token.data());
    }
    return result;
}

Transform ToTransform(string_view name, string_view str) {

    auto tokens = str | std::views::split(' ');
    auto tokenSize = std::ranges::distance(tokens);
    Matrix4x4 result{};
    if (tokenSize == 9) {
        for (const auto [i, token]: tokens | std::views::enumerate) {
            result[i / 3][i % 3] = ToFloat(token.data());
        }
    } else if (tokenSize == 16) {
        for (const auto [i, token]: tokens | std::views::enumerate) {
            result[i / 4][i % 4] = ToFloat(token.data());
        }
    } else if (tokenSize == 3) {
        // defalut translate
        if (name == "translate") {
            for (const auto [i, token]: tokens | std::views::enumerate) {
                result[i][3] = ToFloat(token.data());
            }
        } else if (name == "scale") {
            for (const auto [i, token]: tokens | std::views::enumerate) {
                result[i][i] = ToFloat(token.data());
            }
        } else if (name == "rotate") {
            // rotation
            auto [x, y, z] = ToVector(str);
            x = DegreeToRadian(x);
            y = DegreeToRadian(y);
            z = DegreeToRadian(z);
            auto cx = Cos(x), sx = Sin(x);
            auto xRotation = Matrix4x4(1, 0, 0, 0,
                                       0, cx, -sx, 0,
                                       0, sx, cx, 0,
                                       0, 0, 0, 1);
            auto cy = Cos(y), sy = Sin(y);
            auto yRotation = Matrix4x4(cy, 0, sy, 0,
                                       0, 1, 0, 0,
                                       -sy, 0, cy, 0,
                                       0, 0, 0, 1);
            auto cz = Cos(z), sz = Sin(z);
            auto zRotation = Matrix4x4(cz, -sz, 0, 0,
                                       sz, cz, 0, 0,
                                       0, 0, 1, 0,
                                       0, 0, 0, 1);
            // General 3D Rotations
            // https://en.wikipedia.org/wiki/Rotation_matrix
            result = xRotation * yRotation * zRotation;
        } else {
            throw std::runtime_error("transform size error while parsing");
        }
    } else if (tokenSize != 0) {
        throw std::runtime_error("transform size error while parsing");
    }

    return Transform(result);
}

//  tag <-> name map
enum class ETag : UInt8 {
    EScene = 0,
    ECamera,
    EIntegrator,
    EShape,
    EPrimitive,
    EAggregate,
    EMesh,
    EBxDF,
    ETexture,
    ESampler,
    EFilm,
    ELight,

    // Property
    EBoolean,
    EInteger,
    EFloat,
    EString,
    EColor,
    EPoint,
    EVector,
    ETransform,
    EInvalid,
};

static std::unordered_map<string_view, ETag> ETagMap = {
        {"scene",      ETag::EScene},
        {"camera",     ETag::ECamera},
        {"integrator", ETag::EIntegrator},
        {"shape",      ETag::EShape},
        {"primitive",  ETag::EPrimitive},
        {"aggregate",  ETag::EAggregate},
        {"mesh",       ETag::EMesh},
        {"bxdf",       ETag::EBxDF},
        {"texture",    ETag::ETexture},
        {"sampler",    ETag::ESampler},
        {"film",       ETag::EFilm},
        {"light",      ETag::ELight},
        // Property
        {"boolean",    ETag::EBoolean},
        {"integer",    ETag::EInteger},
        {"float",      ETag::EFloat},
        {"string",     ETag::EString},
        {"color",      ETag::EColor},
        {"point",      ETag::EPoint},
        {"vector",     ETag::EVector},
        {"transform",  ETag::ETransform},
        {"invalid",    ETag::EInvalid},
};

Object *LoadSceneXML(const string_view &filename) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.data());

    // check xml node attribute helper
    auto CheckAttribute = [&](const pugi::xml_node &node, std::unordered_set<string_view> attributes) {
        for (auto attr: node.attributes()) {

            if (!attributes.contains(attr.name())) {

                throw std::runtime_error(std::format("Error while parsing {} : unexpected attribute {} in {}",
                                                     filename.data(), attr.name(), node.name()));
            }
            attributes.erase(attr.name());
        }

        if (!attributes.empty()) {
            throw std::runtime_error(std::format("Error while parsing {} : missing attribute {} in {}",
                                                 filename.data(), *attributes.begin(), node.name()));
        }
    };

    // 递归解析函数
    std::function<Object *(pugi::xml_node &, PropertyList &, ETag)> ParseTag = [&](pugi::xml_node &node,
                                                                                   PropertyList &list,
                                                                                   ETag parentTag) -> Object * {
        if (node.type() == pugi::node_comment || node.type() == pugi::node_declaration) {
            return nullptr;
        }
        if (node.type() != pugi::node_element) {
            throw std::runtime_error(std::format("Error while parsing {} : unexpected content",
                                                 filename.data()));
        }

        if (!ETagMap.contains(node.name())) {
            /* throw exception */
        }

        auto tag = ETagMap[node.name()];

        if (tag == ETag::EScene) {
            node.append_attribute("type") = "scene";
        } else if (tag == ETag::EMesh) {
            if(!node.attribute("type")) {
                node.append_attribute("type") = "mesh";
            }
        } else if (tag == ETag::EFilm) {
            node.append_attribute("type") = "film";
        }

        PropertyList propList;
        std::vector<Object *> children;
        for (auto &ch: node.children()) {
            Object *child = ParseTag(ch, propList, tag);

            if (child != nullptr) {
                children.push_back(child);
            }
        }

        Object *result{nullptr};


        if (tag < ETag::EBoolean) {
            CheckAttribute(node, {"type"});
            result = ObjectFactory::CreateInstance(node.attribute("type").value(), propList);

            for (auto child: children) {
                result->AddChild(child);
                child->SetParent(result);
            }
            result->Initialize();
        } else {
            CheckAttribute(node, {"name", "value"});
            auto name = node.attribute("name").value();
            auto value = node.attribute("value").value();

            switch (tag) {
                case ETag::EBoolean:
                    list.SetBoolean(name, ToBoolean((value)));
                    break;
                case ETag::EInteger:
                    list.SetInteger(name, ToInteger((value)));
                    break;
                case ETag::EFloat:
                    list.SetFloat(name, ToFloat((value)));
                    break;
                case ETag::EString:
                    list.SetString(name, value);
                    break;
                case ETag::EColor:
                    list.SetColor(name, Color3f{ToVector(value)});
                    break;
                case ETag::EPoint:
                    list.SetPoint(name, Point3f{ToVector(value)});
                    break;
                case ETag::EVector:
                    list.SetVector(name, Vector3f{ToVector(value)});
                    break;
                case ETag::ETransform:
                    list.SetTransform(name, ToTransform(name, value));
                    break;
                default:
                    /*throw*/
                    break;
            }
        }

        return result;
    };

    PropertyList list;
    return ParseTag(*doc.begin(), list, ETag::EInvalid);
}

NAMESPACE_END