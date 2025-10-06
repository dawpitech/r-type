/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** main.cpp
*/

#include <cstdlib>
#include <iostream>
#include <Simulation.hpp>

#include "utils/error.hpp"

int main()
{
    try {
        Simulation simulation;
        simulation.runSimulation(true);
    }
    catch (const utils::BaseError& e) {
        std::cerr << e.what() << " in " << e.where() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
