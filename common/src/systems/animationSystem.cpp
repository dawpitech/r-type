//
// EPITECH PROJECT, 2025
// r-type
// File description:
// animationSystem
//

#include "components/Animation.hpp"
#include "components/Sprite.hpp"
#include "flux/core/flux.hpp"

#if IS_CLIENT
    #include "sdlManager.hpp"

    flux::View AnimationSystemView(const flux::ECS& ecs)
    {
        return ecs.GenerateViewFromComponents<component::sprite, component::animation>();
    }

    void AnimationSystem(flux::ECS& ecs, flux::Entity entity)
    {
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

            const auto& [srcX, srcY, srcW, srcH] = anim.frames[anim.currentFrame];

            sprite.destRect.w = srcW;
            sprite.destRect.h = srcH;

            sprite.srcRect.x = srcX;
            sprite.srcRect.y = srcY;
            sprite.srcRect.w = srcW;
            sprite.srcRect.h = srcH;
        }
    }
#else
    void AnimationSystem(flux::ECS& ecs) { return; }
#endif
