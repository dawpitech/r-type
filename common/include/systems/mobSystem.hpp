//
// EPITECH PROJECT, 2025
// r-type
// File description:
// mod system
//

#pragma once

#include "flux/core/flux.hpp"

flux::View MobSystemView(const flux::ECS& ecs);
void MobSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities);
