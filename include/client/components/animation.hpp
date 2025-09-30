//
// EPITECH PROJECT, 2025
// r-type
// File description:
// animation
//

#include <cstdint>
#include <string>
#include <vector>
#include "SDL3/SDL_render.h"
#include "global/utils/error.hpp"

namespace component
{
    struct animation
    {
            std::vector<SDL_Texture> sprites;
            uint8_t actualSprite = 0;

            explicit animation(const std::string& spritesPath)
            {
                if (spritesPath.c_str() == nullptr)
                    throw utils::BaseError("Animation must have a sprites Path", "animation");
            }
    };
} // namespace component
