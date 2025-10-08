//
// EPITECH PROJECT, 2025
// r-type
// File description:
// collision system
//

#pragma once

#include <vector>
#include "flux/core/flux.hpp"

void CollisionSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities);
flux::View CollisionSystemView(const flux::ECS& ecs);
