//
// EPITECH PROJECT, 2025
// r-type
// File description:
// sdlManager
//

#include <string>
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "global/utils/error.hpp"

namespace render
{
    class SDLManager
    {
        public:
            static void renderTexture(SDL_Texture* texture, SDL_FRect srcRect, SDL_FRect destRect)
            {
                SDL_RenderTexture(instance()._renderer, texture, &srcRect, &destRect);
            }

            static void clear() { SDL_RenderClear(instance()._renderer); }

            static void render() { SDL_RenderPresent(instance()._renderer); }

            static SDL_Renderer* getRenderer() { return instance()._renderer; }

            static void init() { SDLManager::instance(); }

            static void setLastTime() { instance()._lastTime = SDL_GetTicks(); }

            static float getDeltaTime() { return instance()._deltaTime; }

            static void handleEvent(bool& running)
            {
                instance()._currentTime = SDL_GetTicks();
                instance()._deltaTime = static_cast<float>(instance()._currentTime - instance()._lastTime) / 1000.0f;
                instance()._lastTime = instance()._currentTime;

                while (SDL_PollEvent(&instance()._event)) {
                    switch (instance()._event.type) {
                        case SDL_EVENT_QUIT:
                            running = false;
                            break;
                        case SDL_EVENT_KEY_DOWN:
                            running = false;
                            break;
                    }
                }
            }
            ~SDLManager()
            {
                if (this->_renderer != nullptr)
                    SDL_DestroyRenderer(this->_renderer);
                if (this->_window != nullptr)
                    SDL_DestroyWindow(this->_window);
                SDL_Quit();
            }

        private:
            SDL_Renderer* _renderer;
            SDL_Window* _window;
            std::string _windowTitle = "La windows";
            SDL_Event _event;
            uint32_t _lastTime = SDL_GetTicks();
            uint32_t _currentTime = SDL_GetTicks();
            float _deltaTime;

            explicit SDLManager()
            {
                if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
                    throw utils::BaseError("Failed to init sdl", "_initSdl");
                }
                this->_window = SDL_CreateWindow(this->_windowTitle.c_str(), 800, 600, 0);
                if (this->_window == nullptr) {
                    throw utils::BaseError("Failed to create window", "_initSdl");
                }
                this->_renderer = SDL_CreateRenderer(this->_window, nullptr);
                if (this->_renderer == nullptr) {
                    throw utils::BaseError("Failed to create Renderer", "_initSdl");
                }
            }

            static SDLManager& instance()
            {
                static SDLManager sdlManager;
                return sdlManager;
            }
    };
} // namespace render
