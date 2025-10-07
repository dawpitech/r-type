//
// EPITECH PROJECT, 2025
// r-type
// File description:
// inputSystem
//

#pragma once

#include <vector>
#include "flux/core/flux.hpp"

void InputSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities);
flux::View InputSystemView(const flux::ECS& ecs);
