//
// EPITECH PROJECT, 2025
// r-type
// File description:
// spriteHandler
//

#pragma once

#include <SDL3/SDL_render.h>
#include <string>

namespace render
{
    struct SpriteData;
    class SpriteHandler
    {
        public:
            SpriteHandler() = default;
            ~SpriteHandler() = default;

            void setSprite(SpriteData &sprite, const std::string& path);

        private:

            void _setPlayerSprite(render::SpriteData &sprite);
            void _setMobSprite(render::SpriteData &sprite);
            void _setPlayerProjectile(render::SpriteData &sprite);
            void _setBackgroundSprite(render::SpriteData &sprite);
    };
} // namespace sprite
