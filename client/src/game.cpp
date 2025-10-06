//
// EPITECH PROJECT, 2025
// r-type
// File description:
// game
//

#include <chrono>
#include <bits/this_thread_sleep.h>

#include "components/Animation.hpp"
#include "components/PlayerInput.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"
#include "game.hpp"
#include "sdlManager.hpp"
#include "systems/animationSystem.hpp"
#include "systems/inputSystem.hpp"
#include "systems/movementSystem.hpp"
#include "systems/renderSystem.hpp"
#include "textureManager.hpp"
#include "spriteHandler.hpp"

void rTypeClient::Game::launchGame()
{
    flux::ECS ecs;
    render::SDLManager::init();
    utils::TextureManager TextureManager;
    sprite::SpriteHandler spriteHandler(TextureManager, render::SDLManager::getRenderer());

    const flux::Entity playerEntity = ecs.newEntity();

    // convert sprite::Rect(s) -> component::animation::Frame(s)
    {
        const auto& spriteMap = spriteHandler.getPlayerSprite().spriteMap;
        std::vector<component::animation::Frame> frames;
        frames.reserve(spriteMap.size());
        for (const auto& r : spriteMap) {
            frames.push_back({ r.srcX, r.srcY, r.destX, r.destY });
        }

        ecs.Add<component::sprite>(playerEntity, component::sprite(spriteHandler.getPlayerSprite().texture, true));
        ecs.Add<component::animation>(playerEntity, component::animation(frames));
    }
    ecs.Add<component::PlayerInput>(playerEntity);
    ecs.Add<component::Transform>(playerEntity, component::Transform(0, 0, 0, 1, 1));
    ecs.Add<component::Velocity>(playerEntity, component::Velocity());

    render::SDLManager::setLastTime();

    constexpr double LOGIC_STEP = 0.01;
    double accumulator = 0.0;
    auto prev = std::chrono::high_resolution_clock::now();

    while (this->_running) {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> frameTime = now - prev;
        prev = now;
        accumulator += frameTime.count();

        while (accumulator > LOGIC_STEP) {
            render::SDLManager::handleEvent(_running);
            InputSystem(ecs);
            MovementSystem(ecs);
            accumulator -= LOGIC_STEP;
        }
        render::SDLManager::clear();
        AnimationSystem(ecs);
        RenderSystem(ecs);
        render::SDLManager::render();

        std::this_thread::sleep_for(std::chrono::nanoseconds(100));
    }
}
