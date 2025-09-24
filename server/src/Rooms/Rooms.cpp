#include "Rooms.hpp"
#include "flux/core/flux.hpp"

Room::Room::Room(const std::uint8_t nbPlayers): _nbPlayers(nbPlayers)
{
    flux::ECS ecs;
}

void Room::Room::update(const std::uint8_t nbFrames)
{
}

void Room::Room::clear(const std::uint8_t nbPlayers)
{
    this->_nbPlayers = nbPlayers;
}
