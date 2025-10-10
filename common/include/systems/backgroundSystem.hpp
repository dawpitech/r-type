//
// EPITECH PROJECT, 2025
// r-type
// File description:
// background System
//

#pragma once

#include "flux/core/flux.hpp"

flux::View BackgroundSystemView(const flux::ECS& ecs);
void BackgroundSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities);
