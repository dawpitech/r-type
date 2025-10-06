//
// EPITECH PROJECT, 2025
// r-type
// File description:
// inputSystem
//

#pragma once

#include "flux/core/flux.hpp"

void InputSystem(flux::ECS& ecs, flux::Entity entity);
flux::View InputSystemView(const flux::ECS& ecs);
