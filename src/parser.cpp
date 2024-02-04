// Created by 44772 on 2024/1/30.
//

#include "util/parser.h"
#include "core/propertylist.h"
#include "core/math.h"
#include <pugixml.hpp>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stdexcept>
#include <format>
#include <print>
#include <ranges>

NAMESPACE_BEGIN

    // Todo: more string check
    Boolean ToBoolean(std::string_view str) {
        return str == "true";
    }

    Integer ToInteger(std::string_view str) {
        return std::stoi(str.data());
    }

    Float ToFloat(std::string_view str) {
        return std::stof(str.data());
    }

    Vector3f ToVector(std::string_view str) {
        auto tokens = str | std::views::split(' ');
        Vector3f result{};
        for (const auto [i, token]: tokens | std::views::enumerate) {
            result[i] = ToFloat(token.data());
        }
        return result;
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

        // Property
        EBoolean,
        EInteger,
        EFloat,
        EString,
        EColor,
        EPoint,
        EVector,
        EInvalid,
    };

    static std::unordered_map<std::string_view, ETag> ETagMap = {
            {"scene",      ETag::EScene},
            {"camera",     ETag::ECamera},
            {"integrator", ETag::EIntegrator},
            {"shape",      ETag::EShape},
            {"primitive",  ETag::EPrimitive},
            {"aggregate",  ETag::EAggregate},
            {"mesh",       ETag::EMesh},
            {"bxdf",       ETag::EBxDF},

            {"vector",     ETag::EVector},
            {"point",      ETag::EPoint},
            {"color",      ETag::EColor},
            {"string",     ETag::EString},
            {"float",      ETag::EFloat},
            {"integer",    ETag::EInteger},
            {"boolean",    ETag::EBoolean},

    };

    Object *LoadSceneXML(const std::string_view &filename) {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(filename.data());

        // check xml node attribute helper
        auto CheckAttribute = [&](const pugi::xml_node &node, std::unordered_set<std::string_view> attributes) {
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
                node.append_attribute("type") = "mesh";
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

            try {
                if (tag < ETag::EBoolean) {
                    CheckAttribute(node, {"type"});
                    result = ObjectFactory::CreateInstance(node.attribute("type").value(), propList);

                    for (auto child: children) {
                        result->AddChild(child);
//                        child->SetParent(result);
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
                        default:
                            /*throw*/
                            break;
                    }
                }
            } catch (...) {
                /*throw*/
            }

            return result;
        };

        PropertyList list;
        return ParseTag(*doc.begin(), list, ETag::EInvalid);
    }

NAMESPACE_END