//
// EPITECH PROJECT, 2025
// r-type
// File description:
// movementSystem
//

#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"

constexpr auto DAMPING_FORCE = 0.98f;

void MovementSystem(flux::ECS& ecs)
{
    const auto view =
        ecs.GenerateViewFromComponents<component::Transform, component::Velocity>();

    for (const std::reference_wrapper entity : ecs.QueryViewNotExclusive(view)) {
        auto& transform = ecs.GetComponent<component::Transform>(entity);
        auto& velocity = ecs.GetComponent<component::Velocity>(entity);

        transform.pos.x += velocity.x;
        transform.pos.y += velocity.y;

        velocity.y *= DAMPING_FORCE;
        velocity.x *= DAMPING_FORCE;
    }
}
