//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Logic for each rooms
//
#include <chrono>
#include <iostream>
#include <thread>

#include "Rooms.hpp"
#include "flux/core/flux.hpp"

Room::Room::Room(std::size_t roomNumber, const std::uint8_t nbPlayers) :
    _roomNumber(roomNumber), _nbPlayers(nbPlayers)
{
    flux::ECS ecs;
}

void Room::Room::update(const std::uint8_t nbFrames)
{
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Room::Room::clear(const std::uint8_t nbPlayers)
{
    this->_nbPlayers = nbPlayers;
}
