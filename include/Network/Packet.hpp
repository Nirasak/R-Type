#pragma once

#include <cstdint>
#include <string>

namespace network {
struct packet {
    uint8_t type;
    std::string data;
};
}