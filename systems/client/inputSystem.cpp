//
// EPITECH PROJECT, 2025
// r-type
// File description:
// inputSystem
//

#include "client/sdlManager.hpp"
#include "flux/core/flux.hpp"
#include "global/components/playerInput.hpp"
#include "global/utils/eventManager.hpp"

void InputSystem(flux::ECS& ecs, flux::Entity entity)
{
    if (!ecs.HasComponents<component::PlayerInput>(entity))
        return;

    auto& event = render::SDLManager::getKeysEvent();
    auto& playerInput = ecs.GetComponent<component::PlayerInput>(entity);
    for (const auto& it : event) {
        if (it.KeyboardEvent->type == utils::KeyEvent::KEY_DOWN)
            switch (it.KeyboardEvent->key) {
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
                    break;
                default:
                    break;
            }
        if (it.KeyboardEvent->type == utils::KeyEvent::KEY_UP)
            switch (it.KeyboardEvent->key) {
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
                    break;
                default:
                    break;
            }
    }
    event.clear();
    SDL_FlushEvent(SDL_EVENT_KEY_DOWN);
}
