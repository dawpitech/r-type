//
// EPITECH PROJECT, 2025
// r-type
// File description:
// game class
//

#pragma once

#include <string>

#include "utils/error.hpp"

namespace rTypeClient
{
    // ReSharper disable once CppClassCanBeFinal
    class GameError : public utils::BaseError
    {
        public:
            explicit GameError(const std::string& msg, const std::string& where) : BaseError(msg, where) {}
    };

    class Game
    {
        public:
            Game() = default;
            ~Game() = default;

            void launchGame();

        private:
            bool _running = true;
    };
} // namespace rTypeClient
