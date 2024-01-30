#include "base/camera.h"
#include <iostream>
#include <print>
#include <pugixml.hpp>
int main() {
    using namespace nrt;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("xgconsole.xml");

    auto camera = ObjectFactory::CreateInstance("perspective", PropertyList());

    std::print("camera class type: {}\n", Object::ClassTypeName(camera->GetClassType()));

    return 0;
}
