//
// EPITECH PROJECT, 2025
// r-type
// File description:
// game class
//

#pragma once

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <exception>
#include <string>
#include "SDL3/SDL_render.h"

namespace rTypeClient
{
    class GameError : public std::exception
    {
        public:
            explicit GameError(const std::string& msg) : _msg(msg) {};

            const char* what() const noexcept override
            {
                return this->_msg.c_str();
            }

        private:
            std::string _msg;
    };

    class Game
    {
        public:
            Game();
            ~Game();

            void launchGame();

        private:
            void _initSdl();
            SDL_Window* _window = nullptr;
            SDL_Renderer* _sdlRenderer = nullptr;
            int _windowWidth = 900;
            int _windowHeight = 900;
            std::string _windowTitle = "R-Type";
    };
} // namespace rTypeClient
