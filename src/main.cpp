#include "core/application.h"
#include <print>

int main() {
    using namespace nrt;
    try {
        Application app;
        app.Run();
    }
    catch (const std::exception &e) {
        std::print("Exception: {}\n", e.what());
    }
    return 0;
}
