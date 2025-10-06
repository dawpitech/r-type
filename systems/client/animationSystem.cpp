//
// EPITECH PROJECT, 2025
// r-type
// File description:
// animationSystem
//

#include "client/components/animation.hpp"
#include "client/components/sprite.hpp"
#include "client/sdlManager.hpp"
#include "flux/core/flux.hpp"

void AnimationSystem(flux::ECS& ecs)
{
    auto view = ecs.GenerateViewFromComponents<component::sprite, component::animation>();

    auto entities = ecs.QueryViewNotExclusive(view);

    for (flux::Entity entity : entities) {
        auto& anim = ecs.GetComponent<component::animation>(entity);
        auto& sprite = ecs.GetComponent<component::sprite>(entity);

        if (!anim.playing || anim.frames.empty())
            continue;
        anim.elapsedTime += render::SDLManager::getDeltaTime();
        if (anim.elapsedTime >= anim.frameTime) {
            anim.elapsedTime = 0.0f;
            anim.currentFrame++;
            if (anim.currentFrame >= static_cast<int>(anim.frames.size())) {
                anim.currentFrame = anim.loop ? 0 : static_cast<int>(anim.frames.size()) - 1;
            }
            sprite.destRect.w = anim.frames[anim.currentFrame].w;
            sprite.destRect.h = anim.frames[anim.currentFrame].h;
            sprite.srcRect = anim.frames[anim.currentFrame];
        }
    }
}
