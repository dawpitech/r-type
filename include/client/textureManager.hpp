//
// EPITECH PROJECT, 2025
// r-type
// File description:
// textureManager
//

#include <string>
#include <unordered_map>
#include <SDL3/SDL_render.h>

namespace utils
{
    class TextureManager
    {
        public:
            TextureManager() = default;
            ~TextureManager();

            SDL_Texture* load(SDL_Renderer* renderer, const std::string& path);
            
        private:
            std::unordered_map<std::string, SDL_Texture*> _textures;
    };
} // namespace utils
