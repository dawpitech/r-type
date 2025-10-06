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

    void AnimationSystem(flux::ECS& ecs)
    {
        const auto view = ecs.GenerateViewFromComponents<component::sprite, component::animation>();

        auto entities = ecs.QueryViewNotExclusive(view);

        for (const flux::Entity& entity : entities) {
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

                const auto& f = anim.frames[anim.currentFrame];

                // map plain Frame -> SDL rects
                sprite.destRect.w = f.srcW;
                sprite.destRect.h = f.srcH;

                sprite.srcRect.x = f.srcX;
                sprite.srcRect.y = f.srcY;
                sprite.srcRect.w = f.srcW;
                sprite.srcRect.h = f.srcH;
            }
        }
    }
#else
    void AnimationSystem(flux::ECS& ecs) {/* server has no animation work to do */}
#endif