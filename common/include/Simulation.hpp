/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Simulation.hpp
*/

#pragma once

#include <memory>

#include "network/datatype.hpp"
#include "vector2.hpp"
#include "flux/core/flux.hpp"

class Simulation
{
    public:
        static void setInitialSimState(flux::ECS& ecs);

        //static void runSimulation(flux::ECS& ecs, std::optional<flux::runtimeHooks> hooks = std::nullopt);
        //void runClientSimulation(std::optional<flux::runtimeHooks> hooks, const std::string& serverIP, uint16_t serverPort);
        //void runServerSimulation(std::optional<flux::runtimeHooks> hooks);

    private:
        enum class PLAYER_TYPE {
            PLAYER_ONE,
            PLAYER_TWO,
            PLAYER_THREE,
            PLAYER_FOUR,
        };

        static void _createEntities(flux::ECS& ecs);
        static void _registerComponent(flux::ECS& ecs);
        static void _createPlayer(flux::ECS& ecs, PLAYER_TYPE type);
        static void _createMob(flux::ECS& ecs, const utils::Vector2<int>& pos);
        //void _setupNetwork(const std::string& serverIp, uint16_t serverPort, std::optional<flux::runtimeHooks> &hooks);
    /*
#ifdef IS_CLIENT
        std::chrono::steady_clock::time_point _lastInputSend;
        std::unique_ptr<client::network::TCPClient> _networkClient;
        std::unique_ptr<client::network::TCPClient> _networkTCPClient;
        std::unique_ptr<client::network::UDPClient> _networkUDPClient;
        network::ClientTCPSentInfo gameInfo;
#endif
*/
};
