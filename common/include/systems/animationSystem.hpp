//
// EPITECH PROJECT, 2025
// r-type
// File description:
// animation system
//

#pragma once

#include "flux/core/flux.hpp"

void AnimationSystem(flux::ECS& ecs, flux::Entity entity);
flux::View AnimationSystemView(const flux::ECS& ecs);
