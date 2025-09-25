//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Logic for each rooms
//
#include <iostream>

#include "Rooms.hpp"
#include "flux/core/flux.hpp"

Room::Room::Room(const std::uint8_t nbPlayers) : _nbPlayers(nbPlayers)
{
    flux::ECS ecs;
}

void Room::Room::update(const std::uint8_t nbFrames)
{
    std::cout << "Hello from room 1" << std::endl;
}

void Room::Room::clear(const std::uint8_t nbPlayers)
{
    this->_nbPlayers = nbPlayers;
}
