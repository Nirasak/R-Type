#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Network/Socket.hpp"
#include "UI/Inputbox.hpp"
#include "UI/Textbox.hpp"
#include "UI/Spritesheet.hpp"
#include "UI/Data.hpp"

class Menu {
    public:
        Menu(sf::RenderWindow &window, network::socket &socket, uint8_t &status);
        ~Menu() = default;

        /**
         * @brief this function init all menu requirements to start from 0
         * 
         */
        void initMenu();

        /**
         * @brief this function is the menu loop that calls all utils functions
         * 
         */
        void loginScreen();

        /**
         * @brief this function is updating cursor texture
         * also select the inputbox selected with cursor 
         * 
         */
        void eventsInputbox();

        /**
         * @brief this function select the next inputbox when user press tab 
         * 
         */
        void checkInputboxSelected();
        
        /**
         * @brief this function is managing all menu events
         * 
         */
        void eventsMenu();

        /**
         * @brief this function update login button texture
         * 
         */
        void eventsLogin();

        /**
         * @brief this function udpate user button texture
         * 
         */
        void eventsUser();

        /**
         * @brief this function send to serv a login request
         * set an error if connection failed
         * go to username stage if connection is setup
         * 
         */
        void tryLogin();

        /**
         * @brief this function send username entered by user to the serv
         * set an error if username is already used
         * go to lobby if username is good
         * 
         */
        void tryUsername();

        /**
         * @brief this function draw the menu on window
         * 
         */
        void displayMenu();

        /**
         * @brief this function draw the username stage and errors
         * 
         */
        void drawUser();

    private:
        sf::RenderWindow &m_window;
        network::socket &m_socket;
        uint8_t &m_status;
        Inputbox m_ip{20, sf::Color::Black, true, 15};
        Inputbox m_port{20, sf::Color::Black, false, 7};
        Inputbox m_user{20, sf::Color::Black, false, 25};
        sf::Color m_black{0, 0, 0, 200};
        sf::Color m_grey{180, 180, 180, 230};
        sf::RectangleShape m_loginrectangle{{400, 450}};
        sf::RectangleShape m_userrectangle{{400, 250}};
        sf::RectangleShape m_ipbox{{330, 25}};
        sf::RectangleShape m_portbox{{330, 25}};
        sf::RectangleShape m_userbox{{330, 25}};
        sf::RectangleShape m_errorbox{{400, 60}};
        Textbox m_texttitle{80, "R-Type", sf::Color::White, {110, 280}};
        Textbox m_textlogin{20, "Server login", sf::Color::White, {90, 410}};
        Textbox m_textip{17, "IP address", sf::Color::White, {80, 470}};
        Textbox m_textport{17, "Port", sf::Color::White, {80, 545}};
        Textbox m_textuser{30, "Enter a username",sf::Color::Black, {840, 450}};
        Textbox m_texterror{20, "Error: IP address or Port is incorrect", sf::Color::Yellow, {850, 900}};
        Spritesheet m_menu{Data::tmenu, {0, 0}, 125, {3600, 10125}, {0, 0, 720, 405}, {2.667, 2.667}, 720};
        Spritesheet m_loginbutton{Data::tloginbutton, {300, 640}, 3, {83, 75}, {0, 0, 83, 25}, {1.3, 1.3}, 83};
        Spritesheet m_userbutton{Data::tloginbutton, {910, 600}, 3, {83, 75}, {0, 0, 83, 25}, {1.3, 1.3}, 83};
        Spritesheet m_loading{Data::tloading, {0, 0}, 12, {612, 51}, {0, 0, 51, 51}, {0.4, 0.4}, 51};
        sf::Event m_event{};
        sf::Font m_font{};
        sf::Clock m_clock{};
        sf::Cursor m_cursor{};
        uint8_t m_logstatus = 0;
        bool m_isloading{};
};