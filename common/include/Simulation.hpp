/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Simulation.hpp
*/

#pragma once

#include "vector2.hpp"
#include "flux/core/flux.hpp"

class Simulation
{
    public:
        static void setInitialClientSimState(flux::ECS& ecs, std::string level);
        static void setInitialServerSimState(flux::ECS& ecs, std::string level);

    private:
        enum class PLAYER_TYPE {
            PLAYER_ONE,
            PLAYER_TWO,
            PLAYER_THREE,
            PLAYER_FOUR,
        };

        static void _createEntities(flux::ECS& ecs, std::string level);
        static void _registerComponent(flux::ECS& ecs);
        static void _registerMenuSystems(flux::ECS& ecs);
        static void _registerGameSystems(flux::ECS& ecs);
        static void _createPlayer(flux::ECS& ecs, PLAYER_TYPE type);
};
