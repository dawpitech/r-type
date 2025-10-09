//
// EPITECH PROJECT, 2025
// r-type
// File description:
// projectile System
//

#pragma once

#include "flux/core/flux.hpp"

flux::View ProjectileSystemView(const flux::ECS& ecs);
void ProjectileSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities);
