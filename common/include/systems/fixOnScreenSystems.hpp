//
// EPITECH PROJECT, 2025
// r-type
// File description:
// fixOnScreen System
//

#pragma once

#include "flux/core/flux.hpp"

flux::View FixOnScreenSystemView(const flux::ECS& ecs);
void FixOnScreenSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities);
