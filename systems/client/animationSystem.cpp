//
// EPITECH PROJECT, 2025
// r-type
// File description:
// animationSystem
//


#include "client/components/sprite.hpp"
#include "client/components/animation.hpp"
#include "flux/core/flux.hpp"

void AnimationSystem(flux::ECS& ecs, flux::Entity entity, float deltaTime) {
    if (!ecs.HasComponents<component::sprite, component::animation>(entity))
        return;

    auto& anim = ecs.GetComponent<component::animation>(entity);
    auto& sprite = ecs.GetComponent<component::sprite>(entity);

    if (!anim.playing || anim.frames.empty())
        return;

    anim.elapsedTime += deltaTime;
    if (anim.elapsedTime >= anim.frameTime) {
        anim.elapsedTime = 0.0f;
        anim.currentFrame++;

        if (anim.currentFrame >= static_cast<int>(anim.frames.size())) {
            anim.currentFrame = anim.loop ? 0 : static_cast<int>(anim.frames.size()) - 1;
        }
    }

    sprite.srcRect = anim.frames[anim.currentFrame];
}
