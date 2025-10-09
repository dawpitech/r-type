//
// EPITECH PROJECT, 2025
// r-type
// File description:
// mob shoot System
//

#pragma once

#include "flux/core/flux.hpp"

flux::View MobShootSystemView(const flux::ECS& ecs);
void MobShootSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities);
