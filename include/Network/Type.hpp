#pragma once

#include <cstdint>

namespace network {
/**
 *  Defines the type of a packet
 *  It's used to simplify packets parsing
 * 
 */
enum type : uint8_t {
    Login,
    Logout,
    PingAlive,
    UsernameOK,
    UsernameKO,
    Lobby,
    Ready,
    NotReady,
    GameStart,
    GameUpdate,
    PlayerInput,
    PlayerDead,
};
}