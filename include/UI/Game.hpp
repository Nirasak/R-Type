#pragma once

#include <SFML/Graphics.hpp>
#include "ECS/SparseArray.hpp"
#include "Network/Socket.hpp"
#include "UI/Spritesheet.hpp"
#include "UI/Textbox.hpp"
#include "UI/Status.hpp"
#include "UI/Data.hpp"

class Game {
    public:
        Game(sf::RenderWindow &window, network::socket &socket, uint8_t &status);
        ~Game() = default;

        /**
         * @brief this function init the game
         * 
         * @param usernames std::vector<std::string> &
         */
        void initGame(const std::vector<std::string> &usernames);

        /**
         * @brief this function is the game loop that calls all utils fonctions
         * 
         */
        void gameScreen(const std::vector<std::string> &usernames);

        /**
         * @brief Get the Serv State object
         * 
         */
        void getServState();

        /**
         * @brief this function is used to parse date received from serv
         * updating game entities status
         * 
         * @param data const std::string & (received from serv)
         */
        void parseData(const std::string &str);

        /**
         * @brief this function update data received from serv
         * 
         * @param parse std::vector<std::string> &
         * @param pos std::vector<std::string> &
         * @param ent size_t
         */
        void updateData(std::vector<std::string> &parse, std::vector<std::string> &pos, size_t ent);

        /**
         * @brief function that remove dead ids in m_display
         * 
         * @param id std::vector<size_t>
         */
        void removeKilled(std::vector<size_t> id);

        /**
         * @brief check user inputs and send them to serv
         * 
         */
        void checkInput();

        /**
         * @brief this function is managing all events during the game
         * 
         */
        void gameEvents();

        /**
         * @brief this function is animating buttons when player is dead
         * 
         */
        void eventsButton();

        /**
         * @brief this function is sending request to serv when player click on a button
         * 
         */
        void eventsPressedButton();

        /**
         * @brief this function is animating sprites
         * 
         */
        void animSprites();

        /**
         * @brief this function is drawing the game on the screen
         * 
         */
        void displayGame();

    private:
        sf::RenderWindow &m_window;
        network::socket &m_socket;
        uint8_t &m_status;
        Spritesheet m_map{Data::tmap, {0, 0}, 1023, {6016, 1080}, {0, 0, 1920, 1080}, {1, 1}, 4};
        sf::Event m_event{};
        sf::Clock m_clock{};
        sf::Clock m_clockinputs{};
        sf::Clock m_clockshots{};
        sf::Clock m_clockanim{};
        sf::Cursor m_cursor{};
        Spritesheet m_lobbybutton{Data::tlobbybutton, {571, 880}, 2, {540, 135}, {0, 0, 270, 135}, {0.7, 0.5}, 270};
        Spritesheet m_disconnectbutton{Data::tdisconnectbuttongame, {1160, 880}, 2, {540, 135}, {0, 0, 270, 135}, {0.7, 0.5}, 270};
        sf::Color m_grey{169, 169, 169, 255};
        sf::Color m_yellow{252, 245, 148, 255};
        sf::Color m_red{255, 80, 37, 255};
        Textbox m_lobbytext{30, "Lobby", m_yellow, {621, 893}};
        Textbox m_disconnecttext{24, "Disconnect", m_red, {1190, 898}};
        Textbox m_deadtext{40, "You are dead!", m_red, {825, 214}};
        Textbox m_playertext[4]{{20, "player1", sf::Color::White, {-100, -100}},
                                {20, "player2", sf::Color::White, {-100, -100}},
                                {20, "player3", sf::Color::White, {-100, -100}},
                                {20, "player4", sf::Color::White, {-100, -100}}};
        sf::RectangleShape m_deadrectangle{{400, 80}};
        bool m_isdead{};
        ecs::sparse_array<Spritesheet> m_display{};
};
