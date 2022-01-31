#include <functional>
#include <string>
#include <chrono>
#include "ECS/Module.hpp"
#include "ECS/Registry.hpp"
#include "ECS/Components.hpp"
#include "ECS/Systems.hpp"
#include "UI/SpriteSize.hpp"
#include "Random.hpp"

class block : public ecs::module {
public:
    block() = default;
    ~block() = default;
    ecs::entity spawn(ecs::registry &reg) override {
        ecs::entity ent;
        for (int i = 0; i < 3; i++) {
            ent = reg.spawn_entity();
            reg.insert_component<ecs::component::label>(ent, "enemy_block");
            reg.insert_component<ecs::component::position>(ent, {1900, tools::random(1000), -5, 0, 0});
            reg.insert_component<ecs::component::pv>(ent, 5);
        }
        return ent;
    }
    static void block_system(ecs::registry &) {
    }
    std::function<void(ecs::registry &)> get_system() override {
        return block_system;
    }
    std::string get_label() override {
        return "enemy_block";
    }
};

extern "C" ecs::module *get_entity() {return new block;}