#include <iostream>

#include "UI/Core.hpp"

int main() {
    try {
        Core app;
        app.loop();
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        return (84);
    }
    return (0);
}
