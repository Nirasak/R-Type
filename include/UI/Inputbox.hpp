#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

constexpr auto DELETE_KEY = 8;
constexpr auto ESCAPE_KEY = 27;

class Inputbox : public sf::Drawable {
    public:
        Inputbox(int size = 15, sf::Color color = sf::Color::White, bool select = false, size_t lim = 0);
        ~Inputbox() = default;

        /**
         * @brief this function is adding char to the end of a sf::String
         * 
         * @param c char to add at the end of the string
         */
        void inputLogic(char c);

        /**
         * @brief Set the Font object
         * 
         * @param font sf::Font to set
         */
        void setFont(const sf::Font &font);

        /**
         * @brief Set the Position object
         * 
         * @param pos sf::Vector2f to set
         */
        void setPosition(sf::Vector2f pos);

        /**
         * @brief Set the Limit object (set the maximum size of the String object)
         * 
         * @param lim size_t to set
         */
        void setLimit(size_t lim);

        /**
         * @brief Set the Selected object
         * 
         * @param sel bool to set
         */
        void setSelected(bool sel);

        /**
         * @brief Set the String object
         * 
         * @param str sf::String to set
         */
        void setString(sf::String str);

        /**
         * @brief Get the Text object
         * 
         * @return sf::String& 
         */
        sf::String &getText();

        /**
         * @brief Get the Text object
         * 
         * @return const sf::String& 
         */
        const sf::String &getText() const;

        /**
         * @brief Get the Selected object
         * 
         * @return true 
         * @return false 
         */
        bool &getSelected();

        /**
         * @brief Get the Inputbox object
         * 
         * @return sf::Text& 
         */
        sf::Text &getInputbox();

        /**
         * @brief this function is updating the sf::Text with the sf::String
         * 
         */
        void updateString();

        /**
         * @brief Drawing the class on the window
         * 
         * @param target sf::RenderTarget window
         * @param states sf::RenderStates Define the states used for drawing to a RenderTarget
         */
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        sf::Text m_inputbox{};
        sf::String m_text{};
        sf::Vector2f m_pos{};
        bool m_selected = false;
        size_t m_limit = 0;
};