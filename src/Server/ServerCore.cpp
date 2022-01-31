#include <filesystem>
#include "ServerCore.hpp"
#include "ECS/Registry.hpp"
#include "Network/Type.hpp"

bool server_core::is_username_taken(const std::map<std::string, client_data> &m, const std::string &ip, const std::string &name)
{
    for (auto &c : m) {
        if (c.first != ip && c.second.username == name) {
            return true;
        }
    }
    return false;
}

void server_core::check_game_start()
{
    bool ok = true;
    for (auto &c : m_serv.get_clients()) {
        if (!m_data[c.get_ip()].is_ready) {
            ok = false;
            m_refresh = std::chrono::system_clock::now();
        }
    }
    if (ok and !m_serv.get_clients().empty() and std::chrono::system_clock::now() > m_refresh + std::chrono::seconds(1)) {
        for (size_t i = 0; i < m_serv.get_clients().size(); i++)
            m_serv.send(m_serv.get_clients()[i], network::GameStart, std::to_string(i), 30);
        init_game();
    }
}

server_core::server_core(unsigned short port) : m_serv(port, 4, network::Login, network::Logout, network::PingAlive)
{
}

void server_core::run() {
    while (true) {
        m_serv.receive();
        check_game_start();
        for (auto &cli : m_serv.get_clients()) {
            auto data = cli.pop_packets();
            for (auto &packet : data) {
                switch (packet.type) {
                    case network::type::UsernameOK: username(cli, packet.data); break;
                    case network::type::Lobby: lobby(cli); break;
                    case network::type::Ready: ready(cli); break;
                    case network::type::NotReady: not_ready(cli); break;
                }
            }
        }
        std::this_thread::yield();
    }
}

void server_core::username(const network::client &cli, const std::string &data)
{
    if (is_username_taken(m_data, cli.get_ip(), data)) {
        m_serv.send(cli, network::UsernameKO, "", 10);
    } else {
        m_serv.send(cli, network::UsernameOK, "", 10);
        m_data[cli.get_ip()] = {data, false};
    }
}

void server_core::lobby(const network::client &cli)
{
    std::string buf;
    for (auto &c : m_serv.get_clients()) {
        if (!m_data[c.get_ip()].username.empty()) {
            buf += m_data[c.get_ip()].username;
            buf += '|';
            if (m_data[c.get_ip()].is_ready)
                buf += '1';
            else
                buf += '0';
            buf += ';';
        }
    }
    m_serv.send(cli, network::Lobby, buf);
}

void server_core::ready(const network::client &cli)
{
    m_data[cli.get_ip()].is_ready = true;
}

void server_core::not_ready(const network::client &cli)
{
    m_data[cli.get_ip()].is_ready = false;
}

void server_core::load_libs() {
    for (auto &p : std::filesystem::recursive_directory_iterator("./lib")) {
        if (p.path().extension() == DyLib::extension) {
            try {
                DyLib lib("./lib/" + p.path().stem().string(), DyLib::extension);
                auto fn = lib.getFunction<ecs::module *()>("get_entity");
                m_libs.push_back(std::move(lib));
                m_mods.emplace_back(fn());
            } catch (...) {
                std::cout << "Failed to load lib: ./lib/" <<  p.path().stem().string() << std::endl;
                continue;
            }
        }
    }
}