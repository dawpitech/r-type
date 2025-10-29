/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** animationSystem.cpp
*/

#include "components/Camera.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"

#include <CameraRaylib.hpp>
#include <components/Transform.hpp>
#include <iostream>

flux::View setCameraSystemView(const flux::ECS& ecs) {
    return ecs.GenerateViewFromComponents<component::Camera, component::Transform, component::Velocity>();
}

void setCameraSystem(flux::ECS &ecs, const std::vector<flux::Entity> &entities) {
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        auto transform = ecs.GetComponent<component::Transform>(entity);
        auto cameraComponent = ecs.GetComponent<component::Camera>(entity);

        auto &camera = CameraRaylib::getCamera();
        camera.target = {transform.pos.x, transform.pos.y};
        camera.offset = {cameraComponent.offsetX, cameraComponent.offsetY};
        camera.rotation = cameraComponent.rotation;
        camera.zoom = cameraComponent.zoom;

    }
}
