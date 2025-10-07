//
// EPITECH PROJECT, 2025
// r-type
// File description:
// MovementSystem
//

#pragma once

#include <vector>
#include "flux/core/flux.hpp"

void MovementSystem(flux::ECS &ecs, const std::vector<flux::Entity>& entities);
flux::View MovementSystemView(const flux::ECS& ecs);
