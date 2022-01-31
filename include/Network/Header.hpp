#pragma once

#include <cstdint>

namespace network {
struct header {
    uint32_t magicValue;
    uint8_t type;
    uint32_t size;
};
}