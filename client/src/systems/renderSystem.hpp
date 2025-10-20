//
// EPITECH PROJECT, 2025
// r-type
// File description:
// render system
//

#pragma once

#include <vector>
#include "flux/core/flux.hpp"

void RenderSystem(flux::ECS &ecs, const std::vector<flux::Entity>& entities);
flux::View RenderSystemView(const flux::ECS& ecs);
