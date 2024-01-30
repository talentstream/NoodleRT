//
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

NAMESPACE_BEGIN

    // 设置 tag name 映射
    enum class ETag {
        EScene,
        ECamera,
        EIntegrator,

        // Property
        EBoolean,
        EInteger,
        EFloat,
        EString,
        EInvalid,
    };

    static std::unordered_map<std::string_view, ETag> ETagMap = {
            {"scene",      ETag::EScene},
            {"camera",     ETag::ECamera},
            {"integrator", ETag::EIntegrator}
    };

    Object *LoadSceneXML(const std::string_view &filename) {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(filename.data());

        // 检查属性函数
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

//                    for(auto child: children){
//                        result->AddChild(child);
//                        ch->SetParent(result);
//                    }
//                    result->Activate();
                } else {
                    CheckAttribute(node, {"name", "value"});
                    switch (tag) {
                        case ETag::EBoolean:
                            propList.SetBoolean(node.attribute("name").value(), node.attribute("value").as_bool());
                            break;
                        case ETag::EInteger:
                            propList.SetInteger(node.attribute("name").value(), node.attribute("value").as_int());
                            break;
                        case ETag::EFloat:
                            propList.SetFloat(node.attribute("name").value(), node.attribute("value").as_float());
                            break;
                        case ETag::EString:
                            propList.SetString(node.attribute("name").value(), node.attribute("value").value());
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