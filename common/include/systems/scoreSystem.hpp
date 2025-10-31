//
// EPITECH PROJECT, 2025
// r-type
// File description:
// Score System
//

#include <vector>
#include "flux/core/flux.hpp"

flux::View ScoreSystemView(const flux::ECS& ecs);
void ScoreSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities);
