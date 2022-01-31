#pragma once

#include <string>
#include <array>
#include <vector>
#include <map>
#include <memory>
#include "DyLib.hpp"
#include "Network/Server.hpp"
#include "ECS/Registry.hpp"
#include "ECS/Module.hpp"

struct client_data {
    std::string username{};
    bool is_ready{};
};

class server_core
{
private:
    std::map<std::string, client_data> m_data{};
    std::vector<DyLib> m_libs{};
    std::vector<std::unique_ptr<ecs::module>> m_mods{};
    std::array<bool, 4> m_already_notified{false, false, false, false};
    network::server m_serv;
    ecs::registry m_reg{};
    std::chrono::system_clock::time_point m_refresh{};

    void check_game_start();
    void init_game();
    void create_players();
    void refresh_game();
    void send_game_state();
    void player_input(size_t player_input, const std::string &data);
    void update_player_position(const network::client &cli, int x, int y);
    bool is_username_taken(const std::map<std::string, client_data> &m, const std::string &ip, const std::string &name);
    void username(const network::client &cli, const std::string &data);
    void lobby(const network::client &cli);
    void ready(const network::client &cli);
    void not_ready(const network::client &cli);
    void game_loop();
    void load_libs();
    bool check_player_alive();
public:
    server_core(unsigned short port);
    ~server_core() = default;
    void run();
};
