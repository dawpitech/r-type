//
// EPITECH PROJECT, 2025
// r-type
// File description:
// healthSystem
//

#pragma once

#include "flux/core/flux.hpp"

void HealthSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities);
flux::View HealthSystemView(const flux::ECS& ecs);
