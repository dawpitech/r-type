/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EndGameSystem.cpp
*/

#include "components/EndGame.hpp"
#include "components/Camera.hpp"
#include "flux/core/flux.hpp"

#include <CameraRaylib.hpp>
#include <components/Transform.hpp>
#include <Functions.hpp>
#include <string>

flux::View endGameSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::Camera, component::Transform, component::EndGame>();
}

void endGameCameraSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        auto transform = ecs.GetComponent<component::Transform>(entity);
        auto endGameComponent = ecs.GetComponent<component::EndGame>(entity);

        if (transform.pos.x >= endGameComponent.endX) {
            std::string text = "End Game";
            int fontSize = 20;
            int textWidth = raylib::MeasureText(text, fontSize);
            int centerX = transform.pos.x - textWidth / 2;
            int centerY = transform.pos.y - fontSize / 2;
            raylib::DrawText(text, centerX, centerY, fontSize, GREEN);
        }
    }
}
