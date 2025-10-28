//
// EPITECH PROJECT, 2025
// r-type
// File description:
// MusicSystem
//

#include <raylib-cpp.hpp>

#include "flux/core/flux.hpp"

flux::View MusicSystemView(const flux::ECS& ecs);
void MusicSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities);
