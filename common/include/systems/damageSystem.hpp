//
// EPITECH PROJECT, 2025
// r-type
// File description:
// damaage system
//

#pragma once

#include <vector>
#include "flux/core/flux.hpp"

void DamageSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities);
flux::View DamageSystemView(const flux::ECS& ecs);
