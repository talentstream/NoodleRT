#include "base/camera.h"
#include <iostream>
#include <print>
#include <pugixml.hpp>
#include "util/parser.h"
int main() {
    using namespace nrt;
//    pugi::xml_document doc;
//    pugi::xml_parse_result result = doc.load_file("../test.xml");
    LoadSceneXML("../test.xml");
//    if(!result)
//    {
//        std::cout << "load xml file failed" << std::endl;
//        return -1;
//    }
//    else {
//        std::cout << "load xml file success" << std::endl;
//    }
//    auto camera = ObjectFactory::CreateInstance("perspective", PropertyList());
//
//    std::print("camera class type: {}\n", Object::ClassTypeName(camera->GetClassType()));

    return 0;
}
