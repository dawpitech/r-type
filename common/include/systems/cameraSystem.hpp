//
// EPITECH PROJECT, 2025
// r-type
// File description:
// camera system
//

#pragma once

#include <vector>
#include "flux/core/flux.hpp"

void CameraSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities);
flux::View CameraSystemView(const flux::ECS& ecs);
