/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** main.cpp
*/

#include <cstdlib>
#include <iostream>
#include "client/game.hpp"
#include "global/utils/error.hpp"

int main()
{
    try {
        rTypeClient::Game game;
        game.launchGame();
    }
    catch (const rTypeClient::GameError& e) {
        std::cerr << e.what() << " in " << e.where() << std::endl;
    }
    catch (const utils::BaseError& e) {
        std::cerr << e.what() << " in " << e.where() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
