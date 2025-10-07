//
// EPITECH PROJECT, 2025
// r-type
// File description:
// collisionSystem
//

#include <vector>
#include "components/Collider.hpp"
#include "flux/core/flux.hpp"

flux::View CollisionSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::collider>();
}


void CollisionSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entity)
{
}
