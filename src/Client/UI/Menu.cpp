/*
** EPITECH PROJECT, 2021
** B-CPP-501-PAR-5-1-rtype-martin.olivier
** File description:
** Menu
*/

#include <thread>
#include "UI/Status.hpp"
#include "UI/Menu.hpp"
#include "UI/Data.hpp"
#include "Network/Type.hpp"
#include "Tools.hpp"

Menu::Menu(sf::RenderWindow &window, network::socket &socket, uint8_t &status) : m_window(window), m_socket(socket), m_status(status)
{
    m_ip.setPosition({85, 500});
    m_port.setPosition({85, 575});
    m_user.setPosition({805, 520});
    m_loginrectangle.setPosition({50, 250});
    m_ipbox.setPosition({80, 500});
    m_portbox.setPosition({80, 575});
    m_userbox.setPosition({800, 520});
    m_userrectangle.setPosition(770, 420);
    m_errorbox.setPosition({820, 883});
    m_loginrectangle.setFillColor(m_black);
    m_ipbox.setFillColor(sf::Color::White);
    m_portbox.setFillColor(sf::Color::White);
    m_userbox.setFillColor(sf::Color::White);
    m_userrectangle.setFillColor(m_grey);
    m_userrectangle.setOutlineColor(sf::Color::White);
    m_userrectangle.setOutlineThickness(1);
    m_errorbox.setFillColor(sf::Color::Black);
    m_errorbox.setOutlineColor(sf::Color::Red);
    m_errorbox.setOutlineThickness(2);
    m_ip.setFont(Data::font);
    m_port.setFont(Data::font);
    m_user.setFont(Data::font);
    m_texttitle.setFont(Data::font);
    m_textlogin.setFont(Data::font);
    m_textip.setFont(Data::font);
    m_textport.setFont(Data::font);
    m_textuser.setFont(Data::font);
    m_texterror.setFont(Data::font);
    m_cursor.loadFromSystem(sf::Cursor::Arrow);
    m_clock.restart();
    Data::menuMusic.setLoop(true);
}

void Menu::initMenu()
{
    Data::menuMusic.play();
    m_logstatus = 0;
    m_ip.setString("");
    m_port.setString("");
    m_user.setString("");
    m_ip.setSelected(true);
    m_port.setSelected(false);
    m_user.setSelected(false);
}

void Menu::loginScreen()
{
    sf::Time elapsed{};

    initMenu();
    while (m_status == MENU && m_window.isOpen()) {
        while (m_window.pollEvent(m_event)) {
            eventsMenu();
        }
        elapsed = m_clock.getElapsedTime();
        if (elapsed.asMilliseconds() > 50) {
            m_menu.animSprite();
            m_loading.animSprite();
            m_loading.setPosition({static_cast<float>(
                sf::Mouse::getPosition(m_window).x) + 10,
                static_cast<float>(sf::Mouse::getPosition(m_window).y) - 20
            });
            displayMenu();
            m_clock.restart();
        }
    }
}

void Menu::eventsMenu()
{
    if (m_event.type == sf::Event::Closed) {
        m_window.close();
        m_status = EXIT;
    }
    eventsInputbox();
    eventsLogin();
    eventsUser();
    if (m_event.type == sf::Event::TextEntered) {
        if ((m_event.text.unicode >= '0' && m_event.text.unicode <= '9') || m_event.text.unicode == '.'
           || m_event.text.unicode == DELETE_KEY || m_event.text.unicode == ESCAPE_KEY) {
            m_ip.inputLogic(static_cast<char>(m_event.text.unicode));
            m_port.inputLogic(static_cast<char>(m_event.text.unicode));
        }
        if ((m_event.text.unicode > 32 && m_event.text.unicode < 137 && m_event.text.unicode != '|'
            && m_event.text.unicode != ';') || m_event.text.unicode == DELETE_KEY || m_event.text.unicode == ESCAPE_KEY) {
            m_user.inputLogic(static_cast<char>(m_event.text.unicode));
        }
    }
    m_window.setMouseCursor(m_cursor);
}

void Menu::eventsInputbox()
{
    sf::Vector2i pmouse = sf::Mouse::getPosition(m_window);

    checkInputboxSelected();    
    if (tools::mouse_is_in_shape(pmouse, m_ipbox)) {
        m_cursor.loadFromSystem(sf::Cursor::Text);
        if (m_event.type == sf::Event::MouseButtonPressed && m_event.mouseButton.button == sf::Mouse::Left && m_logstatus <= 1) {
            m_ip.setSelected(true);
            m_port.setSelected(false);
        }
    } else if (tools::mouse_is_in_shape(pmouse, m_portbox)) {
        m_cursor.loadFromSystem(sf::Cursor::Text);
        if (m_event.type == sf::Event::MouseButtonPressed && m_event.mouseButton.button == sf::Mouse::Left && m_logstatus <= 1) {
            m_ip.setSelected(false);
            m_port.setSelected(true);
        }
    } else if (m_logstatus > 1 && tools::mouse_is_in_shape(pmouse, m_userbox)) {
        m_cursor.loadFromSystem(sf::Cursor::Text);
        if (m_event.type == sf::Event::MouseButtonPressed && m_event.mouseButton.button == sf::Mouse::Left) {
            m_ip.setSelected(false);
            m_port.setSelected(false);
            m_user.setSelected(true);
        }
    } else {
        m_cursor.loadFromSystem(sf::Cursor::Arrow);
    }
}

void Menu::checkInputboxSelected()
{
    if (m_event.type == sf::Event::KeyReleased && m_event.key.code == sf::Keyboard::Tab) {
        if (!m_ip.getSelected() && m_logstatus <= 1) {
            m_ip.setSelected(true);
            m_port.setSelected(false);
        } else if (m_ip.getSelected() && m_logstatus <= 1) {
            m_ip.setSelected(false);
            m_port.setSelected(true);
        } else if (m_logstatus > 1) {
            m_ip.setSelected(false);
            m_port.setSelected(false);
            m_user.setSelected(true);
        }
    }
}

void Menu::eventsLogin()
{
    sf::Vector2i pmouse = sf::Mouse::getPosition(m_window);

    if (m_loginbutton.mouseIsInSprite(pmouse) && m_ip.getText().getSize() > 0 && m_port.getText().getSize() > 0) {
        m_loginbutton.playImage(2);
        m_cursor.loadFromSystem(sf::Cursor::Hand);
        if (m_event.type == sf::Event::MouseButtonPressed && m_event.mouseButton.button == sf::Mouse::Left) {
            tryLogin();
        }
    } else {
        tools::state_button_login({m_ip, m_port}, m_loginbutton);
    }
    if (m_userbutton.mouseIsInSprite(pmouse) && m_user.getText().getSize() > 0) {
        m_userbutton.playImage(2);
        m_cursor.loadFromSystem(sf::Cursor::Hand);
        if (m_event.type == sf::Event::MouseButtonPressed && m_event.mouseButton.button == sf::Mouse::Left) {
            tryUsername();
        }
    } else {
        tools::state_button_login({m_user}, m_userbutton);
    }
}

void Menu::eventsUser()
{
    if (m_event.type == sf::Event::KeyReleased && m_event.key.code == sf::Keyboard::Return) {
        if (m_logstatus >= 2 && m_user.getText().getSize() > 0) {
            tryUsername();
        } else if (m_ip.getText().getSize() > 0 && m_port.getText().getSize() > 0 && m_logstatus <= 1) {
            tryLogin();
            m_ip.setSelected(false);
            m_port.setSelected(false);
        }
    }
}

void Menu::tryLogin()
{
    static bool ishere{};

    if (ishere)
        return;
    ishere = true;
    m_isloading = true;
    std::thread connect_thread([&](){
        std::string ip{m_ip.getText().toAnsiString()};
        unsigned short port = static_cast<unsigned short>(std::atoi(m_port.getText().toAnsiString().c_str()));
        try {
            m_socket.connect(ip, port);
            m_isloading = false;
            m_ip.setSelected(false);
            m_port.setSelected(false);
            m_user.setSelected(true);
            m_logstatus = 2;
        } catch (...) {
            m_isloading = false;
            m_texterror.setString("Error: Could not connect to host");
            m_logstatus = 1;
        }
        ishere = false;
    });
    connect_thread.detach();
}

void Menu::tryUsername()
{
    m_socket.pop_packets();
    std::thread username_thread([&]() {
        m_socket.send(network::UsernameOK, m_user.getText().toAnsiString(), 10);
        std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::seconds(5);
        while (std::chrono::system_clock::now() < deadline) {
            auto packets = m_socket.pop_packets();
            for (auto &p : packets) {
                if (p.type == network::UsernameOK) {
                    m_status = LOBBY;
                    return;
                } else if (p.type == network::UsernameKO) {
                    m_texterror.setString("Error: Username already taken");
                    m_logstatus = 3;
                    return;
                }
            }
        }
        m_logstatus = 0;
    });
    username_thread.detach();
}

void Menu::displayMenu()
{
    m_window.clear();
    m_window.draw(m_menu);
    m_window.draw(m_loginrectangle);
    m_window.draw(m_ipbox);
    m_window.draw(m_portbox);
    m_window.draw(m_texttitle);
    m_window.draw(m_textlogin);
    m_window.draw(m_textip);
    m_window.draw(m_textport);
    m_window.draw(m_loginbutton);
    drawUser();
    m_ip.updateString();
    m_window.draw(m_ip);
    m_port.updateString();
    m_window.draw(m_port);
    if (m_isloading) {
        m_window.draw(m_loading);
    }
    m_window.display();
}

void Menu::drawUser()
{
    if (m_logstatus == 1) {
        m_window.draw(m_errorbox);
        m_window.draw(m_texterror);
    }
    if (m_logstatus == 2) {
        m_window.draw(m_userrectangle);
        m_window.draw(m_textuser);
        m_window.draw(m_userbox);
        m_user.updateString();
        m_window.draw(m_user);
        m_window.draw(m_userbutton);
    }
    if (m_logstatus == 3) {
        m_window.draw(m_errorbox);
        m_window.draw(m_texterror);
        m_window.draw(m_userrectangle);
        m_window.draw(m_textuser);
        m_window.draw(m_userbox);
        m_user.updateString();
        m_window.draw(m_user);
        m_window.draw(m_userbutton);
    }
}