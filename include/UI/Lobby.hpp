#pragma once

#include <mutex>
#include <SFML/Graphics.hpp>
#include "UI/Spritesheet.hpp"
#include "UI/Profile.hpp"
#include "UI/Data.hpp"
#include "Network/Socket.hpp"

class Lobby {
    public:
        Lobby(sf::RenderWindow &window, network::socket &socket, uint8_t &status);
        ~Lobby();

        /**
         * @brief this function init all lobby requirements to start from 0
         * 
         */
        void initLobby();

        /**
         * @brief function called at the end of lobby to stop music, reset the cursor...
         * 
         */
        std::vector<std::string> endLobby();

        /**
         * @brief this function is the lobby loop that calls utils functions
         * 
         */
        std::vector<std::string> lobbyScreen();

        /**
         * @brief this function is managing all lobby events
         * 
         */
        void eventsLobby();

        /**
         * @brief this function is managing ready button animations
         * 
         */
        void eventsReady();

        /**
         * @brief this function is managing disconnect button animations
         * 
         */
        void eventsDisconnect();

        /**
         * @brief this function is managing all buttons events
         * 
         */
        void eventsPressButton();

        /**
         * @brief this function is communicating with the serv
         * getting infos of all other clients connected to the serv: ready or not
         * getting info when game start
         * 
         */
        void reqLobby();

        /**
         * @brief this function is used to parse date received from serv
         * setting a vector of profile (profile = client)
         * 
         * @param data const std::string & (received from serv)
         */
        void parseData(const std::string &data);

        /**
         * @brief function that draw the lobby on the window
         * 
         */
        void displayLobby();

    private:
        sf::RenderWindow &m_window;
        network::socket &m_socket;
        uint8_t &m_status;
        Spritesheet m_menu{Data::tmenu, {0, 0}, 125, {3600, 10125}, {0, 0, 720, 405}, {2.667, 2.667}, 720};
        Spritesheet m_readybutton{Data::treadybutton, {870, 760}, 4, {540, 270}, {0, 0, 270, 135}, {0.7, 0.5}, 270};
        Spritesheet m_disconnectbutton{Data::tdisconnectbutton, {540, 760}, 2, {540, 135}, {0, 0, 270, 135}, {0.7, 0.5}, 270};
        sf::Color m_red{255, 80, 37, 255};
        sf::Color m_green{59, 184, 115, 255};
        Textbox m_lobbytext{60, "Lobby", sf::Color::Black, {880, 150}};
        Textbox m_readytext{30, "Ready", m_red, {920, 773}};
        Textbox m_disconnecttext{24, "Disconnect", sf::Color::Black, {570, 778}};
        sf::RectangleShape m_lobbyrectangle{{1000, 750}};
        sf::Color m_grey{180, 180, 180, 240};
        sf::Event m_event{};
        sf::Cursor m_cursor{};
        sf::Clock m_clock{};
        std::vector<Profile> m_profiles{};
        std::unique_ptr<std::thread> m_thread{};
        std::mutex m_lobbylock{};
        bool m_ready{};
        bool m_lobbyloop = true;
};