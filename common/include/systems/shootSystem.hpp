//
// EPITECH PROJECT, 2025
// r-type
// File description:
// shoot System
//

#pragma once

#include "flux/core/flux.hpp"

flux::View ShootSystemView(const flux::ECS& ecs);
void ShootSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities);
