//
// EPITECH PROJECT, 2025
// r-type
// File description:
// background System
//

#include <SDL3/SDL_rect.h>
#include "components/backgroundComponent.hpp"
#include "components/Sprite.hpp"
#include "flux/core/flux.hpp"

constexpr int SCREEN_SIZE = 960;

flux::View BackgroundSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::background, component::sprite>();
}

void BackgroundSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    using clock = std::chrono::steady_clock;
    static auto prev = clock::now();
    static double accumulator = 0.0;

    constexpr double ANIMATION_STEP = 0.01;

    auto now = clock::now();
    std::chrono::duration<double> frameTime = now - prev;
    prev = now;
    accumulator += frameTime.count();

    while (accumulator >= ANIMATION_STEP) {
        for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
            const flux::Entity& entity = entities[i];

            auto& bg = ecs.GetComponent<component::background>(entity);
            auto& sprite = ecs.GetComponent<component::sprite>(entity);

            if (bg.frames.empty())
                continue;

            if (sprite.srcRect.w == 0 && sprite.srcRect.h == 0) {
                sprite.srcRect.x = bg.frames[0].srcX;
                sprite.srcRect.y = bg.frames[0].srcY;
                sprite.srcRect.w = bg.frames[0].srcw;
                sprite.srcRect.h = bg.frames[0].srch;
            }

            bg.scrollOffset += 1;
            if (bg.scrollOffset > sprite.texture->w - SCREEN_SIZE)
                bg.scrollOffset = 0;
            sprite.srcRect.x = bg.frames[0].srcX + bg.scrollOffset;
            sprite.srcRect.w = SCREEN_SIZE;
            }
        accumulator -= ANIMATION_STEP;
    }
}
