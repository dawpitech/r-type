//
// EPITECH PROJECT, 2025
// r-type
// File description:
// animationSystem
//

#include <iostream>
#include <vector>
#include "components/Animation.hpp"
#include "components/Sprite.hpp"
#include "flux/core/flux.hpp"
#include "sdlManager.hpp"

flux::View AnimationSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::sprite, component::animation>();
}

void AnimationSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        auto& anim = ecs.GetComponent<component::animation>(entity);
        auto& sprite = ecs.GetComponent<component::sprite>(entity);

        if (!anim.playing || anim.frames.empty())
            return;
        anim.elapsedTime += render::SDLManager::getDeltaTime();
        if (anim.elapsedTime >= anim.frameTime) {
            anim.elapsedTime = 0.0f;
            anim.currentFrame++;
            if (anim.currentFrame >= static_cast<int>(anim.frames.size())) {
                anim.currentFrame = anim.loop ? 0 : static_cast<int>(anim.frames.size()) - 1;
            }
            if (!anim.isAnimate)
                anim.currentFrame = 0;
            sprite.destRect.w = anim.frames[anim.currentFrame].w;
            sprite.destRect.h = anim.frames[anim.currentFrame].h;
            sprite.srcRect = anim.frames[anim.currentFrame];
        }
    }
}
