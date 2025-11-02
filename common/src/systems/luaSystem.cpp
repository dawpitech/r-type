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
#include <components/MobStartPosition.hpp>

flux::View LuaSystemView(const flux::ECS &ecs) {
    return ecs.GenerateViewFromComponents<component::Transform, component::Velocity, component::MobStartPosition>();
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
                } else if (componentRequirement == "MobStartPosition" && ecs.HasComponent<component::MobStartPosition>(entity)) {
                    entityComponents["mobStartPosition"] = ecs.GetComponent<component::MobStartPosition>(entity);
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
            } else {
                for (const auto& componentRequirement : system.componentsRequired) {
                    if (componentRequirement == "Transform" && ecs.HasComponent<component::Transform>(entity)) {
                        auto updatedTransform = entityComponents["transform"].get<component::Transform>();
                        ecs.AddOrReplace<component::Transform>(entity, updatedTransform);
                    } else if (componentRequirement == "Velocity" && ecs.HasComponent<component::Velocity>(entity)) {
                        auto updatedVelocity = entityComponents["velocity"].get<component::Velocity>();
                        ecs.AddOrReplace<component::Velocity>(entity, updatedVelocity);
                    } else if (componentRequirement == "MobStartPosition" && ecs.HasComponent<component::MobStartPosition>(entity)) {
                        auto updatedMobStartPosition = entityComponents["mobStartPosition"].get<component::MobStartPosition>();
                        ecs.AddOrReplace<component::MobStartPosition>(entity, updatedMobStartPosition);
                    }
                }
            }
        }
    }
}
