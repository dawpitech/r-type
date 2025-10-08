//
// EPITECH PROJECT, 2025
// r-type
// File description:
// animation system
//

#pragma once

#include <vector>
#include "flux/core/flux.hpp"

void CollisionSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities);
flux::View CollisionSystemView(const flux::ECS& ecs);
