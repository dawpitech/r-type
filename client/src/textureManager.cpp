//
// EPITECH PROJECT, 2025
// r-type
// File description:
// TextureManager
//

#include <SDL3/SDL_render.h>
#include <string>
#include "SDL3_image/SDL_image.h"
#include "global/utils/error.hpp"

#include "client/textureManager.hpp"

SDL_Texture* utils::TextureManager::load(SDL_Renderer* renderer, const std::string& path)
{
    if (this->_textures.count(path) == 0) {
        SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
        if (!texture)
            throw utils::BaseError("failed to load texture", "load");
        this->_textures[path] = texture;
    }
    return _textures[path];
}

utils::TextureManager::~TextureManager()
{
    for (auto& [path, texture] : this->_textures) {
        SDL_DestroyTexture(texture);
    }
}
