#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "UI/Spritesheet.hpp"
#include "UI/Inputbox.hpp"

namespace tools {
    /**
     *  Split a string into a vector<string>
     *
     *  @param str string to be splitted
     *  @param separator character to split at
     *  @return the string splitted into a vector<string>
     */
    inline std::vector<std::string> string_to_vector(const std::string &str, char separator)
    {
        std::vector<std::string> array{};
        std::string temp{};
        size_t len = str.size();

        for (size_t i = 0; i < len; i++) {
            if (str[i] == separator) {
                array.push_back(temp);
                temp.clear();
            }
            else
                temp.push_back(str[i]);
        }
        if (temp.size() != 0) {
            array.push_back(temp);
        }
        return array;
    }
    /**
     *  Check if the mouse is inside the spritesheet
     *
     *  @param pmouse position of the mouse
     *  @param shape spritesheet collisions
     *  @return true if the mouse is in the spritesheet, false otherwise
     */
    inline bool mouse_is_in_shape(sf::Vector2i pmouse, sf::RectangleShape shape)
    {
        if (pmouse.x >= shape.getPosition().x && pmouse.x <= shape.getPosition().x + shape.getSize().x
        && pmouse.y >= shape.getPosition().y && pmouse.y <= shape.getPosition().y + shape.getSize().y) {
            return true;
        }
        return (false);
    }
    /**
     *  If menu's input boxes are filled, the function will animate the login button
     *
     *  @param box vector of input boxes
     *  @param sprite spritesheet to be animated
     */
    inline void state_button_login(const std::vector<Inputbox> &box, Spritesheet &sprite)
    {
        for (auto &c : box) {
            if (c.getText().getSize() == 0) {
                sprite.playImage(0);
                return;
            }
        }
        sprite.playImage(1);
    }
}