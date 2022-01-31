/*
** EPITECH PROJECT, 2021
** B-CPP-501-PAR-5-1-rtype-martin.olivier
** File description:
** Spritesheet
*/

#include "UI/Spritesheet.hpp"

Spritesheet::Spritesheet(sf::Texture &text, sf::Vector2f pos, size_t nb, sf::Vector2f dim, sf::IntRect rect, sf::Vector2f scale, int space)
{
    setTexture(text);
    setPosition(pos);
    setNbSprite(nb);
    setSizeImage(dim);
    setRect(rect);
    setScale(scale);
    setSpace(space);
}

void Spritesheet::setNbSprite(size_t nb)
{
    m_nb = std::move(nb);
}

void Spritesheet::setSizeImage(sf::Vector2f size)
{
    m_size = std::move(size);
}

void Spritesheet::setTexture(const sf::Texture &text)
{
    m_sprite.setTexture(text);
}

void Spritesheet::setPosition(sf::Vector2f pos)
{
    m_pos = std::move(pos);
    m_sprite.setPosition(m_pos);
}

void Spritesheet::setRectSize(sf::IntRect rect)
{
    m_rect.width = rect.width;
    m_rect.height = rect.height;
    m_sprite.setTextureRect(m_rect);
}

void Spritesheet::setRect(sf::IntRect rect)
{
    m_rect = std::move(rect);
    m_sprite.setTextureRect(m_rect);
}

void Spritesheet::setSpace(int space)
{
    m_space = std::move(space);
}

void Spritesheet::setScale(sf::Vector2f scale)
{
    m_scale = std::move(scale);
    m_sprite.setScale(m_scale.x, m_scale.y);
}

size_t &Spritesheet::getCurrentImage()
{
    return (m_current);
}

sf::Vector2f &Spritesheet::getScale()
{
    return (m_scale);
}

sf::IntRect &Spritesheet::getRect()
{
    return (m_rect);
}

size_t &Spritesheet::getNbSprite()
{
    return (m_nb);
}

sf::Vector2f &Spritesheet::getSize()
{
    return (m_size);
}

int &Spritesheet::getSpace()
{
    return (m_space);
}

sf::Vector2f &Spritesheet::getPosition()
{
    return (m_pos);
}

const sf::Texture &Spritesheet::getTexture()
{
    return (*m_sprite.getTexture());
}

bool Spritesheet::mouseIsInSprite(sf::Vector2i pmouse)
{
    if (pmouse.x >= m_pos.x && pmouse.x <= m_pos.x + m_rect.width * m_scale.x
    && pmouse.y >= m_pos.y && pmouse.y <= m_pos.y + m_rect.height * m_scale.y) {
        return (true);
    }
    return (false);
}

void Spritesheet::playImage(size_t nb)
{
    if (nb == 0 || nb >= m_nb) {
        m_current = 0;
        m_rect.left = 0;
        m_rect.top = 0;
    } else {
        m_current = nb;
        m_rect.left = (m_space * nb) % static_cast<int>(m_size.x);
        m_rect.top = ((m_space * nb - m_rect.left) / static_cast<int>(m_size.x)) * m_rect.height;
    }
    m_sprite.setTextureRect(m_rect);    
}

void Spritesheet::animSprite()
{
    playImage(m_current + 1);
}

void Spritesheet::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}
