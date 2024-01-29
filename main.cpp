#include "camera.h"
#include <iostream>
#include <print>
int main() {
    auto camera = ObjectFactory::CreateInstance("perspective");

    std::print("camera class type: {}\n", Object::ClassTypeName(camera->GetClassType()));

    return 0;
}
