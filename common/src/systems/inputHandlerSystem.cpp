//
// EPITECH PROJECT, 2025
// r-type
// File description:
// inputSystem
//

#include <vector>

#include "components/PlayerInput.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"

flux::View InputHandlerSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::PlayerInput, component::Velocity>();
}

void InputHandlerSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    for (const flux::Entity entity : entities) {
        const auto& playerInput = ecs.GetComponent<component::PlayerInput>(entity);
        auto& playerVelocity = ecs.GetComponent<component::Velocity>(entity);

        if (playerInput.move_up)
            playerVelocity.y -= 0.1;

        if (playerInput.move_down)
            playerVelocity.y += 0.1;

        if (playerInput.move_right)
            playerVelocity.x += 0.1;

        if (playerInput.move_left)
            playerVelocity.x -= 0.1;
    }
}
