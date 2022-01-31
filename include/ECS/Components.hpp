#pragma once

#include <string>

namespace ecs {
namespace component {
    struct position {
        int x, y, vx, vy, cur;
    };
    using pv = int;
    using label = std::string;
}
}