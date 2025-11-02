/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EndGameSystem.cpp
*/

#include "components/Camera.hpp"
#include "components/EndGame.hpp"
#include "components/NetworkIdentification.hpp"
#include "components/Player.hpp"
#include "flux/core/flux.hpp"

#include <CameraRaylib.hpp>
#include <Functions.hpp>
#include <components/Transform.hpp>
#include <string>

flux::View endGameSystemView(const flux::ECS &ecs)
{
    return ecs.GenerateViewFromComponents<component::Camera, component::Transform, component::EndGame>();
}

void endGameSystem(flux::ECS &ecs, const std::vector<flux::Entity> &entities)
{
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity &entity = entities[i];
        auto transform = ecs.GetComponent<component::Transform>(entity);
        auto endGameComponent = ecs.GetComponent<component::EndGame>(entity);

        if (transform.pos.x >= endGameComponent.endX) {
            std::string textToDisplay = "End Game\n";

            const auto playerView = ecs.GenerateViewFromComponents<component::Player>();
            const auto allPlayerEntities = ecs.QueryViewNotExclusive(playerView);

            int playerIndex = 0;
            for (auto playerEntity : allPlayerEntities) {
                auto player = ecs.GetComponent<component::Player>(playerEntity);

                bool isLocalPlayer = ecs.HasComponent<component::NetworkIdentification>(playerEntity);

                std::string scoreText =
                    isLocalPlayer ? "You: " : "Player " + std::to_string(playerIndex + 1) + ": ";
                scoreText += std::to_string(player.score);
                scoreText += "\n";

                playerIndex += 1;
                textToDisplay += scoreText;
            }
            int scoreFontSize = 18;
            int scoreTextWidth = raylib::MeasureText(textToDisplay, scoreFontSize);
            int scoreCenterX = transform.pos.x - scoreTextWidth / 2;
            int scoreCenterY = transform.pos.y + 30 + (playerIndex * 25);

            Color textColor = GREEN;
            raylib::DrawText(textToDisplay, scoreCenterX, scoreCenterY, scoreFontSize, textColor);
        }
    }
}
