//
// EPITECH PROJECT, 2025
// r-type
// File description:
// spriteHandler
//

#pragma once

#include <vector>
#include <SDL3/SDL_render.h>

#include "vector2.hpp"

namespace sprite
{
    struct Rect
    {
        float srcX;
        float srcY;
        float destX;
        float destY;
    };

    struct SpriteData
    {
        SDL_Texture *texture;
        utils::Vector2<float> size{0, 0};
        std::vector<Rect> spriteMap;
        utils::Vector2<float> frameSize{0, 0};
    };

    class SpriteHandler
    {
        public:
            SpriteHandler();
            ~SpriteHandler() = default;

            [[nodiscard]] SpriteData getPlayerSprite() const;
            [[nodiscard]] SpriteData getMobSprite() const;

        private:
            SpriteData _playerSprite;
            SpriteData _mobSprite;

            void _setPlayerSprite(SDL_Renderer *renderer);
            void _setMobSprite(SDL_Renderer *renderer);
    };
} // namespace sprite
