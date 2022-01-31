/*
** EPITECH PROJECT, 2021
** B-CPP-501-PAR-5-1-rtype-martin.olivier
** File description:
** Profile
*/

#include "UI/Profile.hpp"
#include <iostream>

Profile::Profile(std::string username, bool rdy, size_t nb)
{
    if (rdy) {
        m_ready.playImage(0);
    } else {
        m_ready.playImage(1);
    }
    m_usernamebox.setString(username);
    m_usernamebox.setFont(Data::font);
    setPosition({550, 300 + static_cast<float>(nb * 100)});
}

void Profile::setPosition(sf::Vector2f pos)
{
    m_pos = std::move(pos);
    m_profilerectangle.setPosition({pos.x, pos.y});
    m_profilerectangle.setFillColor(m_grey);
    m_profilepic.setPosition({m_pos.x + 10, m_pos.y + 5});
    m_usernamebox.setPosition({m_pos.x + 110, m_pos.y + 15});
    m_ready.setPosition({m_pos.x + 730, m_pos.y + 3});
}

void Profile::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_profilerectangle, states);
    target.draw(m_profilepic, states);
    target.draw(m_usernamebox, states);
    target.draw(m_ready, states);
}
