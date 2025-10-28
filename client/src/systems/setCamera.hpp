//
// EPITECH PROJECT, 2025
// r-type
// File description:
// setCamera system
//

#pragma once

#include <vector>
#include "flux/core/flux.hpp"

void setCameraSystem(flux::ECS &ecs, const std::vector<flux::Entity>& entities);
flux::View setCameraSystemView(const flux::ECS& ecs);
