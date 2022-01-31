#pragma once

#include <SFML/Graphics.hpp>
#include "UI/Spritesheet.hpp"
#include "UI/Textbox.hpp"
#include "UI/Data.hpp"

class Profile : public sf::Drawable {
    public:
        Profile(std::string username, bool rdy, size_t nb);
        ~Profile() = default;

        /**
         * @brief Set the Position object
         * set position of all objects
         * 
         * @param pos sf::Vector2f pos
         */
        void setPosition(sf::Vector2f pos);

        /**
         * @brief Get the Username object
         * 
         * @return std::string 
         */
        std::string getUsername() {return (m_usernamebox.getString().toAnsiString());};

        /**
         * @brief Drawing the class on the window
         * 
         * @param target sf::RenderTarget window
         * @param states sf::RenderStates Define the states used for drawing to a RenderTarget
         */
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        Spritesheet m_profilepic{Data::tprofile, {0, 0}, 1, {300, 300}, {0, 0, 300, 300}, {0.2, 0.2}};
        Spritesheet m_ready{Data::tready, {0, 0}, 2, {248, 124}, {0, 0, 124, 124}, {0.5, 0.5}, 124};
        Textbox m_usernamebox{30, "", sf::Color::Black};
        sf::Color m_grey{230, 230, 230, 255};
        sf::RectangleShape m_profilerectangle{{800, 70}};
        sf::Vector2f m_pos{};
};