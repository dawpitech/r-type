//
// EPITECH PROJECT, 2025
// r-type
// File description:
// fixOnScreen System
//

#include <iostream>
#include "components/Camera.hpp"
#include "components/Collider.hpp"
#include "components/FixOnScreen.hpp"
#include "components/Player.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"
#include "flux/core/Serialization.hpp"
#include "vector4.hpp"

flux::View FixOnScreenSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::FixOnScreen>();
}

void FixOnScreenSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    flux::Entity camera;
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        if (ecs.HasComponents<component::Camera, component::Transform, component::Velocity>(entity))
            camera = entity;
    }
    auto transformCamera = ecs.GetComponent<component::Transform>(camera);
    auto cameraComponent = ecs.GetComponent<component::Camera>(camera);

    float cameraLeft = transformCamera.pos.x - cameraComponent.offsetX;
    float cameraRight = transformCamera.pos.x + cameraComponent.offsetX;
    float cameraTop = transformCamera.pos.y - cameraComponent.offsetY;
    float cameraBottom = transformCamera.pos.y + cameraComponent.offsetY;

    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        if (ecs.HasComponents<component::Player, component::Transform, component::Velocity, component::Collider>(
                entity)) {
            auto transformPlayer = ecs.GetComponent<component::Transform>(entity);
            auto velocityPlayer = ecs.GetComponent<component::Velocity>(entity);
            auto colliderPlayer = ecs.GetComponent<component::Collider>(entity);
            float playerLeft = transformPlayer.pos.x + colliderPlayer.srcX;
            float playerRight = transformPlayer.pos.x + colliderPlayer.srcX + colliderPlayer.srcW;
            float playerTop = transformPlayer.pos.y + colliderPlayer.srcY;
            float playerBottom = transformPlayer.pos.y + colliderPlayer.srcY + colliderPlayer.srcH;

            if (playerLeft < cameraLeft) {
                velocityPlayer.x = 1;
            }
            else if (playerRight > cameraRight) {
                velocityPlayer.x = -1;
            }

            if (playerTop < cameraTop) {
                velocityPlayer.y = 1;
            }
            else if (playerBottom > cameraBottom) {
                velocityPlayer.y = -1;
            }
            ecs.AddOrReplace(entity, velocityPlayer);
        }
    }
}
