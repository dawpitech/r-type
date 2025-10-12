//
// EPITECH PROJECT, 2025
// r-type
// File description:
// sdlManager
//

#pragma once

#include <iostream>
#include <map>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "flux/core/Serialization.hpp"
#include "spriteHandler.hpp"
#include "utils/error.hpp"
#include "utils/eventManager.hpp"
#include "vector2.hpp"

namespace render
{
    struct Rect
    {
            float srcX;
            float srcY;
            float srcw;
            float srch;
            REFLECT(srcX, srcY, srcw, srch)
    };

    inline std::ostream& operator<<(std::ostream& stream, const render::Rect& rect)
    {
        stream << rect.srcX << " " << rect.srcY << " " << rect.srcw << " " << rect.srch;
        return stream;
    }

    inline std::istream& operator>>(std::istream& stream, Rect& rect)
    {
        stream >> rect.srcX >> rect.srcY >> rect.srcw >> rect.srch;
        return stream;
    }

    struct SpriteData
    {
            SDL_Texture* texture;
            utils::Vector2<float> size{0, 0};
            std::vector<render::Rect> spriteMap;
            utils::Vector2<float> frameSize{0, 0};
    };

    class SDLManager
    {
        public:
            static void renderTexture(SDL_Texture* texture, const SDL_FRect srcRect, const SDL_FRect destRect)
            {
                SDL_RenderTexture(instance()._renderer, texture, &srcRect, &destRect);
            }

            static void clear() { SDL_RenderClear(instance()._renderer); }

            static void render() { SDL_RenderPresent(instance()._renderer); }

            static SDL_Renderer* getRenderer() { return instance()._renderer; }

            static void init() { SDLManager::instance(); }

            static int getWindowWidth() { return instance()._windowWith; }

            static int getWindowHeight() { return instance()._windowHeight; }

            static void applyColor(SDL_Texture* texture, SDL_Color tint)
            {
                SDL_SetTextureColorMod(texture, tint.r, tint.g, tint.b);
                SDL_SetTextureAlphaMod(texture, tint.a);
                SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
            }

            static std::vector<utils::EventManager>& getKeysEvent() { return instance()._keyEvent; }

            static void handleEvent(bool& running)
            {
                SDL_FlushEvent(SDL_EVENT_KEY_DOWN);
                const std::map<utils::Keys, SDL_Keycode> code{{utils::Keys::KEY_A, SDLK_A},
                                                              {utils::Keys::KEY_B, SDLK_B},
                                                              {utils::Keys::KEY_C, SDLK_C},
                                                              {utils::Keys::KEY_D, SDLK_D},
                                                              {utils::Keys::KEY_E, SDLK_E},
                                                              {utils::Keys::KEY_F, SDLK_F},
                                                              {utils::Keys::KEY_G, SDLK_G},
                                                              {utils::Keys::KEY_H, SDLK_H},
                                                              {utils::Keys::KEY_I, SDLK_I},
                                                              {utils::Keys::KEY_J, SDLK_J},
                                                              {utils::Keys::KEY_K, SDLK_K},
                                                              {utils::Keys::KEY_L, SDLK_L},
                                                              {utils::Keys::KEY_M, SDLK_M},
                                                              {utils::Keys::KEY_N, SDLK_N},
                                                              {utils::Keys::KEY_O, SDLK_O},
                                                              {utils::Keys::KEY_P, SDLK_P},
                                                              {utils::Keys::KEY_Q, SDLK_Q},
                                                              {utils::Keys::KEY_R, SDLK_R},
                                                              {utils::Keys::KEY_S, SDLK_S},
                                                              {utils::Keys::KEY_T, SDLK_T},
                                                              {utils::Keys::KEY_U, SDLK_U},
                                                              {utils::Keys::KEY_V, SDLK_V},
                                                              {utils::Keys::KEY_W, SDLK_W},
                                                              {utils::Keys::KEY_X, SDLK_X},
                                                              {utils::Keys::KEY_Y, SDLK_Y},
                                                              {utils::Keys::KEY_Z, SDLK_Z},
                                                              {utils::Keys::KEY_0, SDLK_0},
                                                              {utils::Keys::KEY_1, SDLK_1},
                                                              {utils::Keys::KEY_2, SDLK_2},
                                                              {utils::Keys::KEY_3, SDLK_3},
                                                              {utils::Keys::KEY_4, SDLK_4},
                                                              {utils::Keys::KEY_5, SDLK_5},
                                                              {utils::Keys::KEY_6, SDLK_6},
                                                              {utils::Keys::KEY_7, SDLK_7},
                                                              {utils::Keys::KEY_8, SDLK_8},
                                                              {utils::Keys::KEY_9, SDLK_9},
                                                              {utils::Keys::SPECIAL_KEY_SPACE, SDLK_SPACE},
                                                              {utils::Keys::SPECIAL_KEY_BACKSPACE, SDLK_BACKSPACE},
                                                              {utils::Keys::SPECIAL_KEY_ENTER, SDLK_RETURN},
                                                              {utils::Keys::ARROW_DOWN, SDLK_DOWN},
                                                              {utils::Keys::ARROW_UP, SDLK_UP},
                                                              {utils::Keys::ARROW_LEFT, SDLK_LEFT},
                                                              {utils::Keys::ARROW_RIGHT, SDLK_RIGHT}};

                while (SDL_PollEvent(&instance()._event)) {
                    switch (instance()._event.type) {
                        case SDL_EVENT_QUIT:
                            running = false;
                            break;
                        case SDL_EVENT_KEY_DOWN:
                            for (auto [fst, snd] : code) {
                                if (instance()._event.key.key == snd) {
                                    utils::EventManager event;
                                    event.type = utils::EventType::KEYBOARD;
                                    event.KeyboardEvent->type = utils::KeyEvent::KEY_DOWN;
                                    event.KeyboardEvent->key = fst;
                                    instance()._keyEvent.push_back(event);
                                    break;
                                }
                            }
                            break;
                        case SDL_EVENT_KEY_UP:
                            for (auto [fst, snd] : code) {
                                if (instance()._event.key.key == snd) {
                                    utils::EventManager event;
                                    event.type = utils::EventType::KEYBOARD;
                                    event.KeyboardEvent->type = utils::KeyEvent::KEY_UP;
                                    event.KeyboardEvent->key = fst;
                                    instance()._keyEvent.push_back(event);
                                    break;
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
            }

            static SpriteData load(const std::string& path)
            {
                if (!instance()._gameSprite.contains(path)) {
                    SpriteData newSprite;
                    newSprite.texture = IMG_LoadTexture(instance()._renderer, path.c_str());

                    if (!newSprite.texture)
                        throw utils::BaseError("failed to load texture", "load");
                    instance().spriteInfo.setSprite(newSprite, path);
                    instance()._gameSprite[path] = newSprite;
                }
                return instance()._gameSprite[path];
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
            std::string _windowTitle = "R-Type";
            SDL_Event _event = {};
            uint32_t _lastTime = SDL_GetTicks();
            uint32_t _currentTime = SDL_GetTicks();
            std::vector<utils::EventManager> _keyEvent;
            std::unordered_map<std::string, SpriteData> _gameSprite;
            SpriteHandler spriteInfo;
            int _windowWith = 960;
            int _windowHeight = 540;

            explicit SDLManager()
            {
                if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
                    throw utils::BaseError(SDL_GetError(), "_initSdl");
                }
                this->_window = SDL_CreateWindow(this->_windowTitle.c_str(), this->_windowWith, this->_windowHeight, 0);
                if (this->_window == nullptr) {
                    throw utils::BaseError(SDL_GetError(), "_initSdl");
                }
                this->_renderer = SDL_CreateRenderer(this->_window, nullptr);
                if (this->_renderer == nullptr) {
                    std::cout << SDL_GetError() << std::endl;
                    throw utils::BaseError(SDL_GetError(), "_initSdl");
                }
            }

            static SDLManager& instance()
            {
                static SDLManager sdlManager;
                return sdlManager;
            }
    };
} // namespace render
