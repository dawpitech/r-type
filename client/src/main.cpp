/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** main.cpp
*/

#include <iostream>
#include "client/game.hpp"

int main()
{
    try {
        rTypeClient::Game game;
        game.launchGame();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
