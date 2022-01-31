/*
** EPITECH PROJECT, 2021
** B-CPP-501-PAR-5-1-rtype-martin.olivier
** File description:
** Lobby
*/

#include <chrono>
#include <thread>
#include "UI/Data.hpp"
#include "UI/Lobby.hpp"
#include "UI/Status.hpp"
#include "Network/Type.hpp"
#include "Tools.hpp"

Lobby::Lobby(sf::RenderWindow &window, network::socket &socket, uint8_t &status) : m_window(window), m_socket(socket), m_status(status)
{
    m_lobbytext.setFont(Data::font);
    m_readytext.setFont(Data::font);
    m_disconnecttext.setFont(Data::font);
    m_lobbyrectangle.setPosition({460, 120});
    m_lobbyrectangle.setFillColor(m_grey);
    m_lobbyrectangle.setOutlineColor(sf::Color::White);
    m_lobbyrectangle.setOutlineThickness(1);
    m_clock.restart();
}

Lobby::~Lobby()
{
    m_lobbyloop = false;
    if (m_thread) {
        m_thread->join();
    }
}

void Lobby::initLobby()
{
    m_lobbyloop = true;
    reqLobby();
    m_cursor.loadFromSystem(sf::Cursor::Arrow);
    m_window.setMouseCursor(m_cursor);
    m_disconnectbutton.playImage(0);
    m_disconnecttext.setColor(sf::Color::Black);
    m_readybutton.playImage(0);
    m_readytext.setColor(m_red);
    Data::menuMusic.play();
}

std::vector<std::string> Lobby::endLobby()
{
    std::vector<std::string> m_usernames{};

    m_lobbyloop = false;
    m_ready = false;
    if (m_thread) {
        m_thread->join();
        m_thread = nullptr;
    }
    Data::menuMusic.stop();
    m_cursor.loadFromSystem(sf::Cursor::Arrow);
    m_window.setMouseCursor(m_cursor);
    for (size_t i = 0; i < m_profiles.size(); i++) {
        m_usernames.push_back(m_profiles[i].getUsername());
    }
    return (m_usernames);
}

std::vector<std::string> Lobby::lobbyScreen()
{
    sf::Time elapsed;

    initLobby();
    while (m_status == LOBBY && m_window.isOpen()) {
        while (m_window.pollEvent(m_event)) {
            eventsLobby();
        }
        elapsed = m_clock.getElapsedTime();
        if (elapsed.asMilliseconds() > 50) {
            m_menu.animSprite();
            displayLobby();
            m_clock.restart();
        }
    }
    return (endLobby());
}

void Lobby::eventsLobby()
{
    if (m_event.type == sf::Event::Closed) {
        m_window.close();
        m_status = EXIT;
    }
    m_cursor.loadFromSystem(sf::Cursor::Arrow);
    eventsReady();
    eventsDisconnect();
    eventsPressButton();
    m_window.setMouseCursor(m_cursor);
}

void Lobby::eventsReady()
{
    sf::Vector2i pmouse = sf::Mouse::getPosition(m_window);

    if (!m_ready) {
        if (m_readybutton.mouseIsInSprite(pmouse)) {
            m_cursor.loadFromSystem(sf::Cursor::Hand);
            m_readytext.setColor(sf::Color::White);
            m_readybutton.playImage(1);
        } else {
            m_readytext.setColor(m_red);
            m_readybutton.playImage(0);
        }
    } else {
        if (m_readybutton.mouseIsInSprite(pmouse)) {
            m_cursor.loadFromSystem(sf::Cursor::Hand);
            m_readytext.setColor(sf::Color::White);
            m_readybutton.playImage(3);
        } else {
            m_readytext.setColor(m_green);
            m_readybutton.playImage(2);
        }
    }
}

void Lobby::eventsDisconnect()
{
    sf::Vector2i pmouse = sf::Mouse::getPosition(m_window);

    if (m_disconnectbutton.mouseIsInSprite(pmouse)) {
        m_cursor.loadFromSystem(sf::Cursor::Hand);
        m_disconnecttext.setColor(sf::Color::White);
        m_disconnectbutton.playImage(1);
    } else {
        m_disconnecttext.setColor(sf::Color::Black);
        m_disconnectbutton.playImage(0);
    }
}

void Lobby::eventsPressButton()
{
    sf::Vector2i pmouse = sf::Mouse::getPosition(m_window);

    if (m_event.type == sf::Event::MouseButtonPressed && m_event.mouseButton.button == sf::Mouse::Left) {
        if (m_readybutton.mouseIsInSprite(pmouse)) {
            m_ready = (m_ready + 1) % 2;
            if (m_ready) {
                m_socket.send(network::Ready, "", 10);
            } else {
                m_socket.send(network::NotReady, "", 10);
            }
        } else if (m_disconnectbutton.mouseIsInSprite(pmouse)) {
            m_socket.disconnect();
            m_status = MENU;
        }
    }
}

void Lobby::reqLobby()
{
    m_thread = std::make_unique<std::thread>([&]() {
        while (m_lobbyloop) {
            try {
                m_socket.send(network::Lobby, "", 10);
            } catch (...) {}
            auto packets = m_socket.pop_packets();
            for (auto &p : packets) {
                if (p.type == network::Lobby) {
                    parseData(p.data);
                } else if (p.type == network::GameStart) {
                    m_status = GAME;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

    });
}

void Lobby::parseData(const std::string &str)
{
    auto profiles = tools::string_to_vector(str, ';');

    std::scoped_lock mtx(m_lobbylock);
    m_profiles.clear();
    for (auto &p : profiles) {
        auto parse = tools::string_to_vector(p, '|');
        m_profiles.emplace_back(parse[0], std::stoi(parse[1]), m_profiles.size());
    }
}

void Lobby::displayLobby()
{
    m_window.clear();
    m_window.draw(m_menu);
    m_window.draw(m_lobbyrectangle);
    m_window.draw(m_lobbytext);
    std::scoped_lock mtx(m_lobbylock);
    for (size_t i = 0; i < m_profiles.size(); i++) {
        m_window.draw(m_profiles[i]);
    }
    m_window.draw(m_disconnectbutton);
    m_window.draw(m_disconnecttext);
    m_window.draw(m_readybutton);
    m_window.draw(m_readytext);
    m_window.display();
}