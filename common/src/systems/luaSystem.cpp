/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** luaBehaviorRefSystem.cpp
*/

#include "systems/luaSystem.hpp"
#include "LuaContext.hpp"

#include <components/Transform.hpp>
#include <components/Velocity.hpp>

flux::View LuaSystemView(const flux::ECS &ecs) {
    return flux::View{};
}

void LuaSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities) {
    //std::cout << "TRACE: C++: LUA SYSTEM TRIGGERED" << std::endl;

    const auto lua = LuaContextStore::getInstance().getLuaContext();
    const auto& luaSystems = LuaContextStore::getInstance().getLuaSystems();
    for (const auto entity : entities) {
        for (const auto& system : luaSystems) {
            sol::table entityComponents = lua->create_table();
            int entityComponentsSize = 0;

            for (const auto& componentRequirement : system.componentsRequired) {
                //std::cout << "Searching for component " << componentRequirement << std::endl;
                if (componentRequirement == "Transform" && ecs.HasComponent<component::Transform>(entity)) {
                    entityComponents["transform"] = ecs.GetComponent<component::Transform>(entity);
                    entityComponentsSize++;
                } else if (componentRequirement == "Velocity" && ecs.HasComponent<component::Velocity>(entity)) {
                    entityComponents["velocity"] = ecs.GetComponent<component::Velocity>(entity);
                    entityComponentsSize++;
                }
            }
            if (system.componentsRequired.size() != entityComponentsSize) {
                //std::cout << "Crapping out: " << system.componentsRequired.size() << "/" << entityComponentsSize++; << std::endl;
                continue;
            }
            if (const auto result = system.fn(entityComponents); !result.valid()) {
                sol::error err = result;
                std::cerr << "ERROR: C++: Error while running lua external code " << err.what() << std::endl;
            }
        }
    }
}
