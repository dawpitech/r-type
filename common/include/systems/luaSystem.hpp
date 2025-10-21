/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** luaBehaviorRefSystem.hpp.h
*/

#pragma once

#include <vector>
#include "flux/core/flux.hpp"

void LuaSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities);
flux::View LuaSystemView(const flux::ECS& ecs);
