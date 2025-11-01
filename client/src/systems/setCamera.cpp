/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** animationSystem.cpp
*/

#include "components/Camera.hpp"
#include "components/NetworkIdentification.hpp"
#include "components/PlayerInput.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"

#include <CameraRaylib.hpp>
#include <components/Transform.hpp>
#include <iostream>
#include <raylib.h>

flux::View setCameraSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::Camera, component::Transform, component::Velocity>();
}

void setCameraSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        auto transform = ecs.GetComponent<component::Transform>(entity);
        auto cameraComponent = ecs.GetComponent<component::Camera>(entity);

        auto& camera = CameraRaylib::getCamera();
        camera.target = {transform.pos.x, transform.pos.y};
        camera.offset = {cameraComponent.offsetX, cameraComponent.offsetY};
        camera.rotation = cameraComponent.rotation;
        camera.zoom = cameraComponent.zoom;

        const auto playerInputview =
            ecs.GenerateViewFromComponents<component::PlayerInput, component::NetworkIdentification>();
        const auto allPlayerInputEntities = ecs.QueryViewNotExclusive(playerInputview);
        for (auto entity : allPlayerInputEntities) {
            auto input = ecs.GetComponent<component::PlayerInput>(entity);
            if (input.colorblind)
                DrawRectangleV({transform.pos.x - cameraComponent.offsetX, 0},
                               {cameraComponent.offsetX * 2, cameraComponent.offsetY * 2}, {0, 64, 128, 80});
        }
    }
}
