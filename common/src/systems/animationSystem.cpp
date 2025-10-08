//
// EPITECH PROJECT, 2025
// r-type
// File description:
// animationSystem
//

#include <vector>
#include "components/Animation.hpp"
#include "components/Sprite.hpp"
#include "flux/core/flux.hpp"

flux::View AnimationSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::sprite, component::animation>();
}

void AnimationSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    using clock = std::chrono::steady_clock;
    static auto prev = clock::now();
    static double accumulator = 0.0;

    constexpr double ANIMATION_STEP = 0.1;

    auto now = clock::now();
    std::chrono::duration<double> frameTime = now - prev;
    prev = now;
    accumulator += frameTime.count();

    while (accumulator >= ANIMATION_STEP) {
        for (auto entity : entities) {
            auto& anim = ecs.GetComponent<component::animation>(entity);
            auto& sprite = ecs.GetComponent<component::sprite>(entity);

            if (!anim.playing || anim.frames.empty())
                continue;

            anim.elapsedTime += ANIMATION_STEP;

            while (anim.elapsedTime >= anim.frameTime) {
                anim.elapsedTime -= anim.frameTime;
                anim.currentFrame++;

                if (anim.currentFrame >= static_cast<int>(anim.frames.size())) {
                    anim.currentFrame = anim.loop ? 0 : static_cast<int>(anim.frames.size()) - 1;
                }
            }

            if (!anim.isAnimate)
                anim.currentFrame = 0;

            sprite.srcRect = anim.frames[anim.currentFrame];
            sprite.destRect.w = anim.frames[anim.currentFrame].w;
            sprite.destRect.h = anim.frames[anim.currentFrame].h;
        }
        accumulator -= ANIMATION_STEP;
    }
}
