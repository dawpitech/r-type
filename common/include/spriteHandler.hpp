//
// EPITECH PROJECT, 2025
// r-type
// File description:
// spriteHandler
//

#pragma once

#include <vector>
#include <SDL3/SDL_render.h>

#include "textureManager.hpp"
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
    };

    class SpriteHandler
    {
        public:
            explicit SpriteHandler(utils::TextureManager &textureManager, SDL_Renderer *renderer);
            ~SpriteHandler() = default;

            [[nodiscard]] SpriteData getPlayerSprite() const;
            [[nodiscard]] SpriteData getMobSprite() const;

        private:
            SpriteData _playerSprite;
            SpriteData _mobSprite;
            utils::TextureManager& _textureManager;

            void _setPlayerSprite(SDL_Renderer *renderer);
            void _setMobSprite(SDL_Renderer *renderer);
    };
} // namespace sprite
