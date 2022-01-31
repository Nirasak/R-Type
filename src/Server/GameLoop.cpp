#include <chrono>
#include "ServerCore.hpp"
#include "Network/Type.hpp"
#include "ECS/Components.hpp"
#include "ECS/Systems.hpp"
#include "Random.hpp"

void server_core::game_loop() {
    std::chrono::system_clock::time_point refresh = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point spawn_mob = std::chrono::system_clock::now();
    while (true) {
        if (m_serv.get_clients().empty())
            throw ecs::exception("[RELOAD] game finished");
        if (!check_player_alive())
            return;
        if (std::chrono::system_clock::now() > refresh + std::chrono::milliseconds(5)) {
            m_serv.receive();
            for (size_t i = 0; i < m_serv.get_clients().size(); i++) {
                auto data = m_serv.get_clients()[i].pop_packets();
                if (!data.empty() and data.back().type == network::type::PlayerInput) {
                    player_input(i, data.back().data);
                }
            }
            refresh_game();
            send_game_state();
            refresh = std::chrono::system_clock::now();
        }
        if (std::chrono::system_clock::now() > spawn_mob + std::chrono::milliseconds(1500)) {
            auto ran = tools::random(m_mods.size());
            m_mods[ran]->spawn(m_reg);
            spawn_mob = std::chrono::system_clock::now();
        }
        std::this_thread::yield();
    }
}

void server_core::player_input(size_t client_pos, const std::string &data) {
    if (data.find("move") == 0)
        ecs::system::move_player(m_reg, client_pos, data[4] - '0', data[5] - '0');
    else if (data.find("shoot") == 0)
        ecs::system::create_friend_shoot(m_reg, client_pos);
    else
        ecs::system::move_player(m_reg, client_pos, 0, 0);
}

void server_core::send_game_state() {
    std::string buffer;
    for (auto &ent : m_reg.get_entities()) {
        buffer += std::to_string(ent);
        buffer += ',';
        buffer += m_reg.get_components<ecs::component::label>()[ent].value();
        buffer += ',';
        auto pos = m_reg.get_components<ecs::component::position>()[ent].value();
        buffer += std::to_string(pos.x);
        buffer += '|';
        buffer += std::to_string(pos.y);
        buffer += '|';
        buffer += std::to_string(pos.vx);
        buffer += '|';
        buffer += std::to_string(pos.vy);
        buffer += '|';
        buffer += std::to_string(pos.cur);
        buffer += ',';
        buffer += std::to_string(m_reg.get_components<ecs::component::pv>()[ent].value());
        buffer += ';';
    }
    for (auto &cli : m_serv.get_clients())
        m_serv.send(cli, network::GameUpdate, buffer); // 12,player0,50|50|20|0|0,1;...
}

void server_core::init_game() {
    m_serv.allow_new_connections(false);
    load_libs();
    m_reg.register_component<ecs::component::label>();
    m_reg.register_component<ecs::component::position>();
    m_reg.register_component<ecs::component::pv>();
    m_reg.add_system(ecs::system::update_velocity);
    m_reg.add_system(ecs::system::entity_killer);
    m_reg.add_system(ecs::system::collision_system);
    ecs::system::create_players(m_reg, m_serv.get_clients().size());
    game_loop();
    m_mods.clear();
    m_libs.clear();
    for (auto &d : m_data)
        d.second.is_ready = false;
    m_reg.destroy();
    m_already_notified = {false, false, false, false};
    m_serv.allow_new_connections(true);
}

void server_core::refresh_game() {
    m_reg.run_systems();
    for (auto &mod : m_mods)
        mod->get_system()(m_reg);
}

bool server_core::check_player_alive() {
    bool all_players_dead = true;
    auto &labels = m_reg.get_components<ecs::component::label>();
    for (size_t i = 0; i < m_serv.get_clients().size(); i++) {
        bool alive = false;
        for (auto ent : m_reg.get_entities()) {
            if (labels[ent] == "player" + std::to_string(i)) {
                all_players_dead = false;
                alive = true;
            }
        }
        if (!alive and m_already_notified[i] == false) {
            m_serv.send(m_serv.get_clients()[i], network::PlayerDead, "", 5);
            m_already_notified[i] = true;
        }
    }
    if (all_players_dead)
        return false;
    return true;
}