#pragma once

#include <SFML/Graphics.hpp>
#include "Network/Type.hpp"
#include "Network/Socket.hpp"
#include "UI/Status.hpp"
#include "UI/Menu.hpp"
#include "UI/Lobby.hpp"
#include "UI/Game.hpp"


class Core {
    public:
        Core();
        ~Core() = default;

        /**
         * @brief this function is the main loop that calls menu, lobby and game
         * 
         */
        void loop();

    private:
        uint8_t m_status = MENU;
        sf::RenderWindow m_window{};
        network::socket m_socket;
        Menu m_menu;
        Lobby m_lobby;
        Game m_game;
};
