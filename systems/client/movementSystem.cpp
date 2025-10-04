//
// EPITECH PROJECT, 2025
// r-type
// File description:
// movementSystem
//

#include "flux/core/flux.hpp"
#include "global/components/Transform.hpp"
#include "global/components/Velocity.hpp"
#include "global/components/playerInput.hpp"

constexpr float SPEED_SCALE = 1000;

void MovementSystem(flux::ECS& ecs, flux::Entity entity)
{
    if (!ecs.HasComponents<component::Transform, component::PlayerInput, component::Velocity>(entity))
        return;

    auto& transform = ecs.GetComponent<component::Transform>(entity);
    auto& input = ecs.GetComponent<component::PlayerInput>(entity);
    auto& velocity = ecs.GetComponent<component::Velocity>(entity);

    if (input.move_up)
        transform.pos.y -= (velocity.speed / SPEED_SCALE);
    if (input.move_down)
        transform.pos.y += (velocity.speed / SPEED_SCALE);
    if (input.move_right)
        transform.pos.x += (velocity.speed / SPEED_SCALE);
    if (input.move_left)
        transform.pos.x -= (velocity.speed / SPEED_SCALE);
}
