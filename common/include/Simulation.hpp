/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Simulation.hpp
*/

#pragma once

#include <memory>

#include "network/datatype.hpp"
#include "vector2.hpp"
#include "flux/core/flux.hpp"

class Simulation
{
    public:
        static void setInitialSimState(flux::ECS& ecs);

    private:
        enum class PLAYER_TYPE {
            PLAYER_ONE,
            PLAYER_TWO,
            PLAYER_THREE,
            PLAYER_FOUR,
        };

        static void _createEntities(flux::ECS& ecs);
        static void _registerComponent(flux::ECS& ecs);
        static void _createPlayer(flux::ECS& ecs, PLAYER_TYPE type);
        static void _createMob(flux::ECS& ecs, const utils::Vector2<int>& pos);
};
