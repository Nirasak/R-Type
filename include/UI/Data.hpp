#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <exception>
#include <map>
#include <functional>
#include "UI/Spritesheet.hpp"

namespace Data {
    class MissingAsset : public std::exception {
    private:
        const std::string m_error{};
    public:
        explicit MissingAsset(const std::string &path) : m_error("missing asset : " + path) {}
        ~MissingAsset() override = default;
        [[nodiscard]] inline const char *what() const noexcept override {return m_error.c_str();}
    };

    inline sf::Texture backgroundTexture{};
    inline sf::Font font{};

    inline sf::Music menuMusic{};
    inline sf::Music gameMusic{};

    inline sf::Texture tmenu{};
    inline sf::Texture tloginbutton{};
    inline sf::Texture tprofile{};
    inline sf::Texture tready{};
    inline sf::Texture treadybutton{};
    inline sf::Texture tdisconnectbutton{};
    inline sf::Texture tloading{};
    inline sf::Texture tmap{};
    inline sf::Texture tmapdead{};
    inline sf::Texture tplayer0{};
    inline sf::Texture tplayer1{};
    inline sf::Texture tplayer2{};
    inline sf::Texture tplayer3{};
    inline sf::Texture tenemy0{};
    inline sf::Texture tenemy1{};
    inline sf::Texture tenemy2{};
    inline sf::Texture tplayershot{};
    inline sf::Texture tenemyshot{};
    inline sf::Texture tblock{};
    inline sf::Texture tlobbybutton{};
    inline sf::Texture tdisconnectbuttongame{};

    inline sf::Sound splayershot{};
    inline sf::SoundBuffer bplayershot{};


    inline std::map<std::string, std::function<Spritesheet()>> factorytexturemap {
        {"player0", {[](){return Spritesheet(tplayer0, {0, 0}, 5, {160, 14}, {0, 0, 32, 14}, {2.5, 2.5}, 32);}}},
        {"player1", {[](){return Spritesheet(tplayer1, {0, 0}, 5, {160, 14}, {0, 0, 32, 14}, {2.5, 2.5}, 32);}}},
        {"player2", {[](){return Spritesheet(tplayer2, {0, 0}, 5, {160, 14}, {0, 0, 32, 14}, {2.5, 2.5}, 32);}}},
        {"player3", {[](){return Spritesheet(tplayer3, {0, 0}, 5, {160, 14}, {0, 0, 32, 14}, {2.5, 2.5}, 32);}}},
        {"enemy_plane", {[](){return Spritesheet(tenemy0, {0, 0}, 8, {168, 24}, {0, 0, 21, 24}, {2.5, 2.5}, 21);}}},
        {"enemy_cyborg", {[](){return Spritesheet(tenemy1, {0, 0}, 4, {128, 31}, {0, 0, 32, 31}, {3, 3}, 32);}}},
        {"enemy2", {[](){return Spritesheet(tenemy1, {0, 0}, 4, {256, 22}, {0, 0, 32, 22}, {2, 2}, 32);}}},
        {"friend_shoot", {[](){return Spritesheet(tplayershot, {0, 0}, 1, {16, 4}, {0, 0, 16, 4}, {2, 2});}}},
        {"enemy_shoot", {[](){return Spritesheet(tenemyshot, {0, 0}, 4, {28, 6}, {0, 0, 7, 6}, {2, 2}, 7);}}},
        {"enemy_block", {[](){return Spritesheet(tblock, {0, 0}, 4, {32, 8}, {0, 0, 8, 8}, {5, 5}, 8);}}}
    };

    /**
     * @brief function that load all assets
     * 
     */
    void load();

    inline void loadAsset(sf::Music &asset, const std::string &path) {
        if (!asset.openFromFile(path))
            throw MissingAsset(path);
    }

    /**
     * @brief function that load content from file and store it inside an asset
     * 
     * @param asset where content will be stored
     * @param path content to load
     * 
     */
    template<typename T>
    inline void loadAsset(T &asset, const std::string &path) {
        if (!asset.loadFromFile(path))
            throw MissingAsset(path);
    }
};