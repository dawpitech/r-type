/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** luaBehaviorRefSystem.cpp
*/

#include "systems/luaSystem.hpp"
#include "LuaContext.hpp"

#include <components/Transform.hpp>

flux::View LuaSystemView(const flux::ECS &ecs) {
    return flux::View{};
}

void LuaSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities) {
    std::cout << "TRACE: C++: LUA SYSTEM TRIGGERED" << std::endl;

    const auto& luaSystems = LuaContextStore::getInstance().getLuaSystems();
    for (const auto entity : entities) {
        for (const auto& system : luaSystems) {
            
        }
    }
}
