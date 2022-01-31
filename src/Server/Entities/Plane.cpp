#include <functional>
#include <string>
#include <chrono>
#include "ECS/Module.hpp"
#include "ECS/Registry.hpp"
#include "ECS/Components.hpp"
#include "ECS/Systems.hpp"
#include "Random.hpp"

class plane : public ecs::module {
public:
    plane() = default;
    ~plane() = default;
    ecs::entity spawn(ecs::registry &reg) override {
        auto ent = reg.spawn_entity();
        auto pos = tools::random(1000);
        reg.insert_component<ecs::component::label>(ent, "enemy_plane");
        reg.insert_component<ecs::component::position>(ent, {1900, pos, -2, -2, 0});
        reg.insert_component<ecs::component::pv>(ent, 1);
        return ent;
    }
    static void system(ecs::registry &reg) {
        auto &labels = reg.get_components<ecs::component::label>();
        auto &pos = reg.get_components<ecs::component::position>();
        static std::chrono::system_clock::time_point refresh = std::chrono::system_clock::now();

        for (auto &e : reg.get_entities()) {
            if (labels[e].value() == "enemy_plane") {
                if (pos[e].value().cur > 200)
                    pos[e].value().vy = -2;
                else if (pos[e].value().cur < -200)
                    pos[e].value().vy = +2;
            }
        }

        if (std::chrono::system_clock::now() < refresh + std::chrono::seconds(2))
            return;
        for (auto &e : reg.get_entities()) {
            if (labels[e].value() == "enemy_plane")
                ecs::system::create_enemy_shoot(reg, e);
        }
        refresh = std::chrono::system_clock::now();
    }
    std::function<void(ecs::registry &)> get_system() override {
        return system;
    }
    std::string get_label() override {
        return "enemy_plane";
    }
};

extern "C" ecs::module *get_entity() {return new plane;}