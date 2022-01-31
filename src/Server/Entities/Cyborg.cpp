#include <functional>
#include <string>
#include <chrono>
#include "ECS/Module.hpp"
#include "ECS/Registry.hpp"
#include "ECS/Components.hpp"
#include "ECS/Systems.hpp"
#include "Random.hpp"

class cyborg : public ecs::module {
public:
    cyborg() = default;
    ~cyborg() = default;
    ecs::entity spawn(ecs::registry &reg) override {
        auto ent = reg.spawn_entity();
        auto pos = tools::random(1000);
        reg.insert_component<ecs::component::label>(ent, "enemy_cyborg");
        reg.insert_component<ecs::component::position>(ent, {1900, pos, -3, 0, 0});
        reg.insert_component<ecs::component::pv>(ent, 3);
        return ent;
    }
    static void system(ecs::registry &reg) {
        static int status = 0;
        auto &labels = reg.get_components<ecs::component::label>();
        static std::chrono::system_clock::time_point refresh = std::chrono::system_clock::now();

        if (status == 0 and std::chrono::system_clock::now() < refresh + std::chrono::seconds(3))
            return;
        if (status == 3) {
            status = 0;
            refresh = std::chrono::system_clock::now();
            return;
        }
        if (std::chrono::system_clock::now() > refresh + std::chrono::milliseconds(200)) {
            for (auto &e : reg.get_entities()) {
                if (labels[e].value() == "enemy_cyborg")
                    ecs::system::create_enemy_shoot(reg, e, -6);
            }
            refresh = std::chrono::system_clock::now();
            status += 1;
        }
    }
    std::function<void(ecs::registry &)> get_system() override {
        return system;
    }
    std::string get_label() override {
        return "enemy_cyborg";
    }
};

extern "C" ecs::module *get_entity() {return new cyborg;}