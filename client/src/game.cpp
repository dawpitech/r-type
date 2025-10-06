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

    ecs.registerSystem(InputSystem, InputSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(MovementSystem, MovementSystemView(ecs), flux::systemType::LOGIC);
    ecs.registerSystem(AnimationSystem, AnimationSystemView(ecs), flux::systemType::RENDER);
    ecs.registerSystem(RenderSystem, RenderSystemView(ecs), flux::systemType::RENDER);

    flux::runtimeHooks hooks = {
        .hookBeforeLogic = flux::make_hook(render::SDLManager::handleEvent, std::ref(ecs.getMasterRunState())),
        .hookBeforeRender = []{ render::SDLManager::clear(); },
        .hookAfterRender = []{ render::SDLManager::render(); },
    };

    render::SDLManager::setLastTime();
    ecs.handExecution(hooks);
}
