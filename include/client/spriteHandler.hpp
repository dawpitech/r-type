//
// EPITECH PROJECT, 2025
// r-type
// File description:
// spriteHandler
//

#pragma once

#include <vector>
#include "SDL3/SDL_render.h"
#include "global/vector2.hpp"
#include "client/textureManager.hpp"

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
            std::vector<sprite::Rect> spriteMap;
    };

    class SpriteHandler
    {
        public:
            explicit SpriteHandler(utils::TextureManager &textureManager, SDL_Renderer *renderer);
            ~SpriteHandler() = default;

            [[nodiscard]] sprite::SpriteData getPlayerSprite() const;
            [[nodiscard]] sprite::SpriteData getMobSprite() const;

        private:
            sprite::SpriteData _playerSprite;
            sprite::SpriteData _mobSprite;
            utils::TextureManager& _textureManager;

            void _setPlayerSprite(SDL_Renderer *renderer);
            void _setMobSprite(SDL_Renderer *renderer);
    };
} // namespace sprite
