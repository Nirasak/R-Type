#include <gtest/gtest.h>
#include "ECS/Entity.hpp"
#include "ECS/Components.hpp"
#include "ECS/Systems.hpp"
#include "ECS/Registry.hpp"

TEST(ecs, entity_tests)
{
    ecs::registry reg;
    reg.register_component<std::string>();
    EXPECT_EQ(reg.has_component<std::string>(), true);
    auto e1 = reg.spawn_entity();
    auto e2 = reg.spawn_entity();
    auto e3 = reg.spawn_entity();
    reg.kill_entity(e2);
    EXPECT_EQ(reg.get_entities().size(), 2);
    EXPECT_EQ(reg.get_entities().front(), e1);
    EXPECT_EQ(reg.get_entities().back(), e3);
    EXPECT_EQ(reg.spawn_entity(), e2);
}

TEST(ecs, component_tests)
{
    using position = std::pair<int, int>;
    auto pos = std::make_pair<int, int>(200, 200);
    ecs::registry reg;
    reg.register_component<std::string>();
    reg.register_component<int>();
    reg.register_component<position>();
    EXPECT_EQ(reg.has_component<position>(), true);
    auto ent = reg.spawn_entity();
    reg.emplace_component<std::string>(ent, "player");
    reg.insert_component<int>(ent, 100);
    reg.emplace_component<position>(ent, pos);

    EXPECT_EQ(reg.get_components<std::string>()[ent], "player");
    EXPECT_EQ(reg.get_components<int>()[ent], 100);
    EXPECT_EQ(reg.get_components<position>()[ent], pos);
}

TEST(ecs, system_tests)
{
    using position = std::pair<int, int>;
    auto pos = std::make_pair<int, int>(200, 200);
    ecs::registry reg;
    reg.register_component<std::string>();
    reg.register_component<int>();
    reg.register_component<position>();
    EXPECT_EQ(reg.has_component<position>(), true);
    auto ent = reg.spawn_entity();
    reg.emplace_component<std::string>(ent, "player");
    reg.insert_component<int>(ent, 100);
    reg.emplace_component<position>(ent, pos);

    reg.add_system([&](ecs::registry &reg) {
        auto &tags = reg.get_components<std::string>();
        auto &pvs = reg.get_components<int>();
        for (auto ent : reg.get_entities()) {
            if (tags[ent] == "player")
                pvs[ent].value() -= 5;
        }
    });

    reg.add_system([&](ecs::registry &reg) {
        auto &tags = reg.get_components<std::string>();
        auto &poss = reg.get_components<position>();
        for (auto ent : reg.get_entities()) {
            if (tags[ent] == "player")
                poss[ent].value().first += 5;
        }
    });

    EXPECT_EQ(reg.get_components<std::string>()[ent], "player");
    EXPECT_EQ(reg.get_components<int>()[ent], 100);
    EXPECT_EQ(reg.get_components<position>()[ent], pos);

    for (size_t i = 0; i < 4; i++)
        reg.run_systems();

    pos.first += 20;
    EXPECT_EQ(reg.get_components<std::string>()[ent], "player");
    EXPECT_EQ(reg.get_components<int>()[ent], 80);
    EXPECT_EQ(reg.get_components<position>()[ent], pos);
}