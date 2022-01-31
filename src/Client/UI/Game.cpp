/*
** EPITECH PROJECT, 2021
** B-CPP-501-PAR-5-1-rtype-martin.olivier
** File description:
** Game
*/

#include "UI/Game.hpp"
#include "UI/SpriteSize.hpp"
#include "UI/Data.hpp"
#include "Network/Type.hpp"
#include "Tools.hpp"

Game::Game(sf::RenderWindow &window, network::socket &socket, uint8_t &status) : m_window(window), m_socket(socket), m_status(status)
{
    m_lobbytext.setFont(Data::font);
    m_disconnecttext.setFont(Data::font);
    m_deadtext.setFont(Data::font);
    for (int i = 0; i < 4; i++) {
        m_playertext[i].setFont(Data::font);
    }
    m_deadrectangle.setPosition({760, 200});
    m_deadrectangle.setFillColor(m_grey);
    m_deadrectangle.setOutlineColor(m_red);
    m_deadrectangle.setOutlineThickness(1);
    m_clock.restart();
    m_clockinputs.restart();
    m_clockshots.restart();
}

void Game::initGame(const std::vector<std::string> &usernames)
{
    Data::gameMusic.play();
    m_isdead = false;
    m_map.setTexture(Data::tmap);
    eventsButton();
    for (size_t i = 0; i < usernames.size(); i++) {
        m_playertext[i].setString(usernames[i]);
    }
}

void Game::gameScreen(const std::vector<std::string> &usernames)
{
    sf::Time elapsed{};

    initGame(usernames);
    while (m_status == GAME && m_window.isOpen()) {
        while (m_window.pollEvent(m_event)) {
            gameEvents();
        }
        if (!m_isdead) {
            checkInput();
        }
        elapsed = m_clock.getElapsedTime();
        if (elapsed.asMilliseconds() > 30) {
            getServState();
            m_map.animSprite();
            displayGame();
            m_clock.restart();
        }
    }
    Data::gameMusic.stop();
}

void Game::checkInput()
{
    sf::Time elapsed{};
    char c[2] = {'0', '0'};

    elapsed = m_clockinputs.getElapsedTime();
    if (elapsed.asMilliseconds() > 30) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            c[0] -= 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            c[0] += 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            c[1] -= 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            c[1] += 1;
        }
        m_socket.send(network::PlayerInput, "move" + std::string(c));
        m_clockinputs.restart();
    }
    elapsed = m_clockshots.getElapsedTime();
    if (elapsed.asMilliseconds() > 400 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        m_socket.send(network::PlayerInput, "shoot");
        Data::splayershot.play();
        m_clockshots.restart();
    }
}

void Game::getServState()
{
    network::packet lastp{};
    auto packets = m_socket.pop_packets();

    for (auto &p : packets) {
        if (p.type == network::type::GameUpdate) {
            lastp = std::move(p);
        }
        if (p.type == network::type::PlayerDead) {
            m_map.setTexture(Data::tmapdead);
            m_isdead = true;
        }
    }
    if (lastp.type == network::type::GameUpdate) {
        parseData(lastp.data);
    }
}

void Game::parseData(const std::string &str)
{
    sf::Time elapsed = m_clockanim.getElapsedTime();
    auto entities = tools::string_to_vector(str, ';');
    std::vector<size_t> id{};
    size_t ent = 0;

    for (int i = 0; i < 4; i++) {
        m_playertext[i].setPosition({-100, -100});
    }
    for (auto &p : entities) {
        auto parse = tools::string_to_vector(p, ',');
        auto pos = tools::string_to_vector(parse[2], '|');
        ent = std::stoul(parse[0]);
        if (m_display[ent] == std::nullopt) {
            m_display.insert_at(ent, Data::factorytexturemap[parse[1]]());
            m_display[ent].value().setPosition({std::stof(pos[0]), std::stof(pos[1])});
        } else {
            updateData(parse, pos, ent);
        }
        id.push_back(ent);
    }
    if (elapsed.asMilliseconds() > 200) {
        m_clockanim.restart();
    }
    removeKilled(id);
}

void Game::updateData(std::vector<std::string> &parse, std::vector<std::string> &pos, size_t ent)
{
    sf::Time elapsed = m_clockanim.getElapsedTime();

    m_display[ent].value().setTexture(Data::factorytexturemap[parse[1]]().getTexture());
    m_display[ent].value().setPosition({std::stof(pos[0]), std::stof(pos[1])});
    m_display[ent].value().setRectSize(Data::factorytexturemap[parse[1]]().getRect());
    m_display[ent].value().setSpace(Data::factorytexturemap[parse[1]]().getSpace());
    m_display[ent].value().setNbSprite(Data::factorytexturemap[parse[1]]().getNbSprite());
    m_display[ent].value().setScale(Data::factorytexturemap[parse[1]]().getScale());
    m_display[ent].value().setSizeImage(Data::factorytexturemap[parse[1]]().getSize());
    if (parse[1].find("player") == 0) {
        m_playertext[parse[1][6] - '0'].setPosition({std::stof(pos[0]), std::stof(pos[1]) - 30});
    }
    if (elapsed.asMilliseconds() > 200) {
        if (parse[1].find("player") == 0) {
            auto velocityy = std::stoi(pos[3]);
            auto &imagenb = m_display[ent].value().getCurrentImage();
            if (velocityy < 0 && imagenb < 4) {
                m_display[ent].value().animSprite();
            }
            if (velocityy == 0) {
                m_display[ent].value().playImage(2);
            }
            if (velocityy > 0 && imagenb > 0) {
                m_display[ent].value().playImage(imagenb - 1);
            }
        } else {
            m_display[ent].value().animSprite();
        }
    }
}

void Game::removeKilled(std::vector<size_t> id)
{
    for (size_t i = 0; i < m_display.size(); i++) {
        if (m_display[i] != std::nullopt && std::find(std::begin(id), std::end(id), i) == std::end(id)) {
            m_display[i] = std::nullopt;
        }
    }
}

void Game::gameEvents()
{
    if (m_event.type == sf::Event::Closed) {
        m_window.close();
        m_status = EXIT;
    }
    m_cursor.loadFromSystem(sf::Cursor::Arrow);
    if (m_isdead) {
        eventsButton();
        eventsPressedButton();
    }
    m_window.setMouseCursor(m_cursor);
}

void Game::eventsButton()
{
    sf::Vector2i pmouse = sf::Mouse::getPosition(m_window);

    if (m_lobbybutton.mouseIsInSprite(pmouse)) {
        m_cursor.loadFromSystem(sf::Cursor::Hand);
        m_lobbytext.setColor(m_grey);
        m_lobbybutton.playImage(1);
    } else {
        m_lobbytext.setColor(m_yellow);
        m_lobbybutton.playImage(0);
    }
    if (m_disconnectbutton.mouseIsInSprite(pmouse)) {
        m_cursor.loadFromSystem(sf::Cursor::Hand);
        m_disconnecttext.setColor(m_grey);
        m_disconnectbutton.playImage(1);
    } else {
        m_disconnecttext.setColor(m_red);
        m_disconnectbutton.playImage(0);
    }
}

void Game::eventsPressedButton()
{
    sf::Vector2i pmouse = sf::Mouse::getPosition(m_window);

    if (m_event.type == sf::Event::MouseButtonPressed && m_event.mouseButton.button == sf::Mouse::Left) {
        if (m_lobbybutton.mouseIsInSprite(pmouse)) {
            m_status = LOBBY;
        }
        if (m_disconnectbutton.mouseIsInSprite(pmouse)) {
            m_socket.disconnect();
            m_status = MENU;
        }
    }
}

void Game::displayGame()
{
    m_window.clear();
    m_window.draw(m_map);
    for (int i = 0; i < 4; i++) {
        m_window.draw(m_playertext[i]);
    }
    for (auto &p : m_display) {
        if (p != std::nullopt) {
            m_window.draw(p.value());
        }
    }
    if (m_isdead) {
        m_window.draw(m_deadrectangle);
        m_window.draw(m_deadtext);
        m_window.draw(m_lobbybutton);
        m_window.draw(m_lobbytext);
        m_window.draw(m_disconnectbutton);
        m_window.draw(m_disconnecttext);
    }
    m_window.display();
}