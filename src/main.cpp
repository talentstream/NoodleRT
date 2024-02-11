#include "core/application.h"
#include <print>
#include <iostream>

int main() {
    using namespace nrt;
    try {
        Application app;
        app.Run();
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
