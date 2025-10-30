//
// EPITECH PROJECT, 2025
// r-type
// File description:
// cameraSystem
//

#include "components/Camera.hpp"
#include "components/EndGame.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"

flux::View CameraSystemView(const flux::ECS& ecs)
{
    return ecs
        .GenerateViewFromComponents<component::Camera, component::Transform, component::Velocity, component::EndGame>();
}

void CameraSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        auto endGame = ecs.GetComponent<component::EndGame>(entity);
        auto transform = ecs.GetComponent<component::Transform>(entity);
        if (transform.pos.x < endGame.endX) {
            auto velocity = ecs.GetComponent<component::Velocity>(entity);
            velocity.x += 0.01f;
            ecs.AddOrReplace<component::Velocity>(entity, velocity);
        }
    }
}
