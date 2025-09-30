//
// EPITECH PROJECT, 2025
// r-type
// File description:
// animation
//

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>

#include "client/spriteHandler.hpp"

namespace component
{
    struct animation
    {
            std::vector<SDL_Rect> frames;
            int currentFrame = 0;
            float frameTime = 0.1f;
            float elapsedTime = 0.0f;

            bool loop = true;
            bool playing = true;

            explicit animation(const std::vector<::sprite::Rect>& rect)
            {
                for (auto it : rect) {
                    SDL_Rect rect{it.srcX, it.srcY, it.destX, it.destY};
                    frames.push_back(rect);
                }
            }
    };
} // namespace component
