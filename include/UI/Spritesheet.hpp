#pragma once

#include <SFML/Graphics.hpp>

class Spritesheet : public sf::Drawable {
    public:
        Spritesheet(sf::Texture &text, sf::Vector2f pos = {0, 0}, size_t nb = 1, sf::Vector2f dim = {1920, 1080}, sf::IntRect rect = {0, 0, 1920, 1080}, sf::Vector2f scale = {1, 1}, int space = 0);
        ~Spritesheet() = default;

        /**
         * @brief Set the Nb Sprite object
         * 
         * @param nb size_t (nb of sprites on image)
         */
        void setNbSprite(size_t nb);

        /**
         * @brief Set the Size Image object
         * 
         * @param size sf::Vector2f (size of image)
         */
        void setSizeImage(sf::Vector2f size);

        /**
         * @brief Set the Texture object
         * 
         * @param text const sf::Texture & (with a load texture)
         */
        void setTexture(const sf::Texture &text);

        /**
         * @brief Set the Position object
         * 
         * @param pos sf::Vector2f (pos of texture on window)
         */
        void setPosition(sf::Vector2f pos);

        /**
         * @brief Set the Rect object (only width and height)
         * 
         * @param rect 
         */
        void setRectSize(sf::IntRect rect);

        /**
         * @brief Set the Rect object
         * 
         * @param rect sf::IntRect (rect of the sprite on image)
         */
        void setRect(sf::IntRect rect);

        /**
         * @brief Set the Space object
         * 
         * @param space int (space between all sprites on image)
         */
        void setSpace(int space);

        /**
         * @brief Set the Scale object
         * 
         * @param scale sf::Vector2f (scale of sprite on window)
         */
        void setScale(sf::Vector2f scale);

        /**
         * @brief Get the Current Image object
         * 
         * @return size_t& 
         */
        size_t &getCurrentImage();

        /**
         * @brief Get the Nb Sprite object
         * 
         * @return size_t& 
         */
        size_t &getNbSprite();

        /**
         * @brief Get the Scale object
         * 
         * @return sf::Vector2f& 
         */
        sf::Vector2f &getScale();

        /**
         * @brief Get the Rect object
         * 
         * @return sf::IntRect& 
         */
        sf::IntRect &getRect();

        /**
         * @brief Get the Size object
         * 
         * @return sf::Vector2f& 
         */
        sf::Vector2f &getSize();

        /**
         * @brief Get the Space object
         * 
         * @return int& 
         */
        int &getSpace();

        /**
         * @brief Get the Position object
         * 
         * @return sf::Vector2f& 
         */
        sf::Vector2f &getPosition();

        /**
         * @brief Get the Texture object
         * 
         * @return sf::Texture& 
         */
        const sf::Texture &getTexture();

        /**
         * @brief function that check if the cursor is in the sprite
         * return true if the cursor is in the sprite, false otherwise
         * 
         * @param pmouse sf::Vector2i (pos of cursor on windows)
         * @return true 
         * @return false 
         */
        bool mouseIsInSprite(sf::Vector2i pmouse);

        /**
         * @brief function that play an exact sprite
         * 
         * @param nb size_t (sprite nb wanted to display on window)
         */
        void playImage(size_t nb);

        /**
         * @brief function that call play image with the next current sprite
         * 
         */
        void animSprite();

        /**
         * @brief Drawing the class on the window
         * 
         * @param target sf::RenderTarget window
         * @param states sf::RenderStates Define the states used for drawing to a RenderTarget
         */
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        sf::Sprite m_sprite{};
        sf::Vector2f m_pos{};
        sf::IntRect m_rect{};
        sf::Vector2f m_scale{};
        sf::Vector2f m_size{};
        int m_space = 0;
        size_t m_nb = 0;
        size_t m_current = 0;

};
