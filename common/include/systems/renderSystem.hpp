//
// EPITECH PROJECT, 2025
// r-type
// File description:
// render system
//

#pragma once

#include "flux/core/flux.hpp"

void RenderSystem(flux::ECS &ecs, flux::Entity entity);
flux::View RenderSystemView(const flux::ECS& ecs);
