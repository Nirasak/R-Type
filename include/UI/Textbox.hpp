#pragma once

#include <SFML/Graphics.hpp>

class Textbox : public sf::Drawable {
    public:
        Textbox(size_t size = 20, sf::String str = "", sf::Color color = sf::Color::White, sf::Vector2f pos = {0, 0});
        ~Textbox() = default;

        /**
         * @brief Set the Size object
         * 
         * @param size size_t (set size of text)
         */
        void setSize(size_t size);

        /**
         * @brief Set the String object
         * 
         * @param str sf::String (string to draw)
         */
        void setString(sf::String str);

        /**
         * @brief Set the Color object
         * 
         * @param color sf::Color (color of the text)
         */
        void setColor(sf::Color color);

        /**
         * @brief Set the Position object
         * 
         * @param pos sf::Vector2f (position of the text on window)
         */
        void setPosition(sf::Vector2f pos);

        /**
         * @brief Set the Outline Color object
         * 
         * @param color sf::Color (outline color)
         * @param size float (size of outline)
         */
        void setOutlineColor(sf::Color color, float size);

        /**
         * @brief Set the Font object
         * 
         * @param font const sf::Font & (font of text)
         */
        void setFont(const sf::Font &font);

        /**
         * @brief Get the String object
         * 
         * @return sf::String& 
         */
        sf::String &getString(void);

        /**
         * @brief Get the Position object
         * 
         * @return sf::Vector2f& 
         */
        sf::Vector2f &getPosition(void);

        /**
         * @brief Drawing the class on the window
         * 
         * @param target sf::RenderTarget window
         * @param states sf::RenderStates Define the states used for drawing to a RenderTarget
         */
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        sf::Text m_text{};
        sf::String m_str{};
        sf::Vector2f m_pos{};
};
