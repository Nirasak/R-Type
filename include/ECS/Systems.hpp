#pragma once

#include "Registry.hpp"
#include "Components.hpp"
#include "UI/SpriteSize.hpp"
#include <iostream>

namespace ecs {
namespace system {
    inline void create_players(registry &reg, size_t client_size) {
        for (size_t i = 0; i < client_size; i++) {
            auto ent = reg.spawn_entity();
            reg.insert_component<ecs::component::label>(ent, "player" + std::to_string(i));
            reg.insert_component<ecs::component::position>(ent, {0, 100 + static_cast<int>(100 * i), 0, 0, 0});
            reg.insert_component<ecs::component::pv>(ent, 1);
        }
    }

    inline void move_player(registry &reg, size_t client_pos, int x, int y) {
        auto &positions = reg.get_components<ecs::component::position>();
        auto &tags = reg.get_components<ecs::component::label>();
        for (auto &e : reg.get_entities()) {
            if (tags[e].value() == "player" + std::to_string(client_pos)) {
                auto &val = positions[e].value();
                if (y > 0)
                    val.vy = 1;
                else if (y < 0)
                    val.vy = -1;
                else
                    val.vy = 0;
                val.x += x * 15;
                val.y += y * 15;
                if (val.x < 0)
                    val.x = 0;
                if (val.y < 0)
                    val.y = 0;
                if (val.x > 1856)
                    val.x = 1856;
                if (val.y > 1052)
                    val.y = 1052;
                return;
            }
        }
    }

    inline void create_friend_shoot(registry &reg, size_t client_pos) {
        auto &tags = reg.get_components<ecs::component::label>();
        auto &positions = reg.get_components<ecs::component::position>();
        for (auto player : reg.get_entities()) {
            if (tags[player].value() == "player" + std::to_string(client_pos)) {
                auto ent = reg.spawn_entity();
                auto player_pos = positions[player].value();
                reg.insert_component<ecs::component::label>(ent, "friend_shoot");
                reg.insert_component<ecs::component::position>(ent,
                    {player_pos.x + static_cast<int>(Data::sprite_data["player"].first.x * Data::sprite_data["player"].second.x),
                    player_pos.y + static_cast<int>((Data::sprite_data["player"].first.y * Data::sprite_data["player"].second.y) / 2), 20, 0, 0});
                reg.insert_component<ecs::component::pv>(ent, 1);
                return;
            }
        }
    }

    inline void create_enemy_shoot(registry &reg, size_t enemy_ent, int speed = -5) {
        auto &label = reg.get_components<ecs::component::label>();
        auto &positions = reg.get_components<ecs::component::position>();

        auto ent = reg.spawn_entity();
        auto enemy_pos = positions[enemy_ent].value();
        reg.insert_component<ecs::component::label>(ent, "enemy_shoot");
        reg.insert_component<ecs::component::position>(ent,
        {enemy_pos.x, enemy_pos.y + static_cast<int>((Data::sprite_data[label[enemy_ent].value()].first.y
        * Data::sprite_data[label[enemy_ent].value()].second.y) / 2), speed, 0, 0});
        reg.insert_component<ecs::component::pv>(ent, 1);
    }

    inline void update_velocity(registry &reg) {
        auto &positions = reg.get_components<ecs::component::position>();
        auto &tags = reg.get_components<ecs::component::label>();
        auto &pvs = reg.get_components<ecs::component::pv>();
        for (auto &e : reg.get_entities()) {
            if (tags[e].value().find("player") != 0) {
                positions[e].value().x += positions[e].value().vx;
                positions[e].value().y += positions[e].value().vy;
                positions[e].value().cur += positions[e].value().vy;
                if (positions[e].value().x > 1920 or positions[e].value().x < -100)
                    pvs[e].value() = 0;
            }
        }
    }

    inline void entity_killer(registry &reg) {
        auto &pvs = reg.get_components<ecs::component::pv>();
        for (auto &e : reg.get_entities()) {
            if (pvs[e].value() <= 0) {
                reg.kill_entity(e);
                return;
            }
        }
    }

    bool is_collision(registry &reg, entity e1, entity e2) {
        auto &label = reg.get_components<ecs::component::label>();
        auto &l1 = label[e1].value();
        auto &l2 = label[e2].value();
        auto &positions = reg.get_components<ecs::component::position>();
        auto &pos1 = positions[e1].value();
        auto &pos2 = positions[e2].value();
        float rect1[2] = {Data::sprite_data[l1].first.x * Data::sprite_data[l1].second.x,
                        Data::sprite_data[l1].first.y * Data::sprite_data[l1].second.y};
        float rect2[2] = {Data::sprite_data[l2].first.x * Data::sprite_data[l2].second.x,
                        Data::sprite_data[l2].first.y * Data::sprite_data[l2].second.y};

        if (pos1.x >= pos2.x && pos1.x <= pos2.x + rect2[0] && pos1.y >= pos2.y && pos1.y <= pos2.y + rect2[1])
            return true;
        if (pos1.x + rect1[0] >= pos2.x && pos1.x + rect1[0] <= pos2.x + rect2[0] && pos1.y >= pos2.y && pos1.y <= pos2.y + rect2[1])
            return true;
        if (pos1.x >= pos2.x && pos1.x <= pos2.x + rect2[0] && pos1.y + rect1[1] >= pos2.y && pos1.y + rect1[1] <= pos2.y + rect2[1])
            return true;
        if (pos1.x + rect1[0] >= pos2.x && pos1.x + rect1[0] <= pos2.x + rect2[0] && pos1.y + rect1[1] >= pos2.y && pos1.y + rect1[1] <= pos2.y + rect2[1])
            return true;
        if (pos2.x >= pos1.x && pos2.x <= pos1.x + rect1[0] && pos2.y >= pos1.y && pos2.y <= pos1.y + rect1[1])
            return true;
        if (pos2.x + rect2[0] >= pos1.x && pos2.x + rect2[0] <= pos1.x + rect1[0] && pos2.y >= pos1.y && pos2.y <= pos1.y + rect1[1])
            return true;
        if (pos2.x >= pos1.x && pos2.x <= pos1.x + rect1[0] && pos2.y + rect2[1] >= pos1.y && pos2.y + rect2[1] <= pos1.y + rect1[1])
            return true;
        if (pos2.x + rect2[0] >= pos1.x && pos2.x + rect2[0] <= pos1.x + rect1[0] && pos2.y + rect2[1] >= pos1.y && pos2.y + rect2[1] <= pos1.y + rect1[1])
            return true;
        return false;
    }

    inline void collision_system(registry &reg) {
        auto &labels = reg.get_components<ecs::component::label>();
        auto &pvs = reg.get_components<ecs::component::pv>();
        for (auto e1 : reg.get_entities()) {
            for (auto e2 : reg.get_entities()) {
                if (e1 != e2 and is_collision(reg, e1, e2)) {
                    if ((labels[e1].value().find("enemy") == 0 and labels[e2].value().find("player") == 0) and
                        labels[e1].value().find("enemy_shoot") != 0) {
                        pvs[e1].value() -= 1;
                        pvs[e2].value() -= 1;
                    }
                    if ((labels[e1].value().find("enemy_shoot") == 0 and labels[e2].value().find("player") == 0) and
                        labels[e2].value().find("player_shoot") != 0) {
                        pvs[e1].value() -= 1;
                        pvs[e2].value() -= 1;
                    }
                    if ((labels[e1].value().find("friend_shoot") == 0 and labels[e2].value().find("enemy") == 0) and
                        labels[e2].value().find("enemy_shoot") != 0) {
                        pvs[e1].value() -= 1;
                        pvs[e2].value() -= 1;
                    }
                }
            }
        }
    }
}
}