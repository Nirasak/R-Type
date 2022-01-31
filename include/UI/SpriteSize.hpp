#pragma once

#include <map>
#include <string>
#include <utility>

struct vec2float
{
    float x, y;
};

namespace Data {
inline std::map<std::string, std::pair<vec2float, vec2float>> sprite_data {
    {"player", {{32, 14}, {2.5, 2.5}}},
    {"player0", {{32, 14}, {2.5, 2.5}}},
    {"player1", {{32, 14}, {2.5, 2.5}}},
    {"player2", {{32, 14}, {2.5, 2.5}}},
    {"player3", {{32, 14}, {2.5, 2.5}}},
    {"enemy_plane", {{21, 24}, {2.5, 2.5}}},
    {"enemy_cyborg", {{32, 31}, {3, 3}}},
    {"enemy2", {{32, 22}, {2, 2}}},
    {"friend_shoot", {{16, 4}, {2, 2}}},
    {"enemy_shoot", {{7, 6}, {2, 2}}},
    {"enemy_block", {{8, 8}, {5, 5}}}
};
}