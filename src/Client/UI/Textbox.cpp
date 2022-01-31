/*
** EPITECH PROJECT, 2021
** B-CPP-501-PAR-5-1-rtype-martin.olivier
** File description:
** Textbox
*/

#include "UI/Textbox.hpp"

Textbox::Textbox(size_t size, sf::String str, sf::Color color, sf::Vector2f pos)
{
    setSize(size);
    setString(str);
    setColor(color);
    setPosition(pos);
}

void Textbox::setSize(size_t size)
{
    m_text.setCharacterSize(size);
}

void Textbox::setString(sf::String str)
{
    m_str = std::move(str);
    m_text.setString(m_str);
}

void Textbox::setColor(sf::Color color)
{
    m_text.setFillColor(color);
}

void Textbox::setPosition(sf::Vector2f pos)
{
    m_pos = std::move(pos);
    m_text.setPosition(m_pos);
}

void Textbox::setOutlineColor(sf::Color color, float size)
{
    m_text.setOutlineColor(color);
    m_text.setOutlineThickness(size);
}

void Textbox::setFont(const sf::Font &font)
{
    m_text.setFont(font);
}

sf::String &Textbox::getString(void)
{
    return (m_str);
}


sf::Vector2f &Textbox::getPosition(void)
{
    return (m_pos);
}

void Textbox::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_text, states);
}