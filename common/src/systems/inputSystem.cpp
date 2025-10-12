//
// EPITECH PROJECT, 2025
// r-type
// File description:
// inputSystem
//

#include <iostream>
#include <vector>
#include "components/NetworkIdentification.hpp"
#include "components/PlayerInput.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"
#include "utils/eventManager.hpp"

#if IS_CLIENT
#include "../../include/sdlManager.hpp"
#endif

constexpr auto MAX_VERTICAL_SPEED = 10;
constexpr auto MAX_HORIZONTAL_SPEED = 10;

flux::View InputSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::PlayerInput, component::Velocity>();
}

void InputSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        auto& playerInput = ecs.GetComponent<component::PlayerInput>(entity);
        auto& playerVelocity = ecs.GetComponent<component::Velocity>(entity);

#if IS_CLIENT
        if (ecs.HasComponent<component::NetworkIdentification>(entity)) {
            // const auto& netId = ecs.GetComponent<component::NetworkIdentification>(entity);
            // if (localUUID != netId.uuid)
            //     continue;
            for (const auto& [type, KeyboardEvent] : render::SDLManager::getKeysEvent()) {
                if (KeyboardEvent->type == utils::KeyEvent::KEY_DOWN)
                    switch (KeyboardEvent->key) {
                        case utils::Keys::ARROW_DOWN:
                            playerInput.move_down = true;
                            break;
                        case utils::Keys::ARROW_UP:
                            playerInput.move_up = true;
                            break;
                        case utils::Keys::ARROW_LEFT:
                            playerInput.move_left = true;
                            break;
                        case utils::Keys::ARROW_RIGHT:
                            playerInput.move_right = true;
                            std::cout << "Right\n";
                            break;
                        // case utils::Keys::SPECIAL_KEY_SPACE:
                        //     playerInput.shoot = true;
                        //     break;
                        default:
                            break;
                    }
                if (KeyboardEvent->type == utils::KeyEvent::KEY_UP)
                    switch (KeyboardEvent->key) {
                        case utils::Keys::ARROW_DOWN:
                            playerInput.move_down = false;
                            break;
                        case utils::Keys::ARROW_UP:
                            playerInput.move_up = false;
                            break;
                        case utils::Keys::ARROW_LEFT:
                            playerInput.move_left = false;
                            break;
                        case utils::Keys::ARROW_RIGHT:
                            playerInput.move_right = false;
                            std::cout << "Stop\n";
                            break;
                        case utils::Keys::SPECIAL_KEY_SPACE:
                            playerInput.shoot = true;
                            std::cout << "Shoot\n";
                            break;
                        default:
                            break;
                    }
            }
            render::SDLManager::getKeysEvent().clear();
            SDL_FlushEvent(SDL_EVENT_KEY_DOWN);
        }
#endif

        if (playerInput.move_up)
            playerVelocity.y -= 0.1;

        if (playerInput.move_down)
            playerVelocity.y += 0.1;

        if (playerVelocity.y < -MAX_VERTICAL_SPEED)
            playerVelocity.y = -MAX_VERTICAL_SPEED;
        if (playerVelocity.y > MAX_VERTICAL_SPEED)
            playerVelocity.y = MAX_VERTICAL_SPEED;

        if (playerInput.move_right)
            playerVelocity.x += 0.1;

        if (playerInput.move_left)
            playerVelocity.x -= 0.1;

        if (playerVelocity.x < -MAX_HORIZONTAL_SPEED)
            playerVelocity.x = -MAX_HORIZONTAL_SPEED;
        if (playerVelocity.x > MAX_HORIZONTAL_SPEED)
            playerVelocity.x = MAX_HORIZONTAL_SPEED;
    }
}
