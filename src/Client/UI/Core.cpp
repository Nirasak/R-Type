/*
** EPITECH PROJECT, 2021
** B-CPP-501-PAR-5-1-rtype-martin.olivier
** File description:
** UI
*/

#include "UI/Core.hpp"
#include "UI/Data.hpp"

Core::Core() : m_socket(5000, network::Login, network::Logout, network::PingAlive), m_menu{m_window, m_socket, m_status},
                m_lobby{m_window, m_socket, m_status}, m_game{m_window, m_socket, m_status}
{
    Data::load();
    m_window.create(sf::VideoMode(1920, 1080), "R-Type");
    m_window.setFramerateLimit(60);
}

void Core::loop()
{
    std::vector<std::string> usernames{};

    while (m_status != EXIT && m_window.isOpen()) {
        if (m_status == MENU) {
            m_menu.loginScreen();
        }
        if (m_status == LOBBY) {
            usernames = m_lobby.lobbyScreen();
        }
        if (m_status == GAME) {
            m_game.gameScreen(usernames);
        }
    }
}