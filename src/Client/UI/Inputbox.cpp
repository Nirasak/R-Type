/*
** EPITECH PROJECT, 2021
** B-CPP-501-PAR-5-1-rtype-martin.olivier
** File description:
** Inputbox
*/

#include "UI/Inputbox.hpp"

Inputbox::Inputbox(int size, sf::Color color, bool select, size_t lim)
{
    m_inputbox.setCharacterSize(size);
    m_inputbox.setFillColor(color);
    setSelected(select);
    setLimit(lim);
}

void Inputbox::inputLogic(char c)
{
    if (c == ESCAPE_KEY) {
        m_selected = false;
    }
    if (!m_selected) {
        return;
    }
    if (c != DELETE_KEY && (m_text.getSize() < m_limit || m_limit == 0)) {
        m_text += c;
    }
    if (c == DELETE_KEY && m_text.getSize() > 0) {
        m_text.erase(m_text.getSize() - 1);
    }
    m_inputbox.setString(m_text);
}


void Inputbox::setFont(const sf::Font &font)
{
    m_inputbox.setFont(font);
}

void Inputbox::setPosition(sf::Vector2f pos)
{
    m_pos = std::move(pos);
    m_inputbox.setPosition(m_pos);
}

void Inputbox::setLimit(size_t lim)
{
    m_limit = std::move(lim);
}

void Inputbox::setSelected(bool sel)
{
    m_selected = std::move(sel);
}

void Inputbox::setString(sf::String str)
{
    m_text = std::move(str);
}

sf::String &Inputbox::getText()
{
    return (m_text);
}

const sf::String &Inputbox::getText() const
{
    return (m_text);
}

bool &Inputbox::getSelected()
{
    return (m_selected);
}

sf::Text &Inputbox::getInputbox()
{
    return (m_inputbox);
}

void Inputbox::updateString()
{
    if (m_selected) {
        m_inputbox.setString(m_text + "_");
    } else {
        m_inputbox.setString(m_text);
    }
}

void Inputbox::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_inputbox, states);
}