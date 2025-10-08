/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Simulation.hpp
*/

#pragma once

#include <memory>

#include "flux/core/flux.hpp"

#ifdef IS_CLIENT
  #include "network/TCPClient.hpp"
#endif

class Simulation
{
    public:
        void runSimulation(std::optional<flux::runtimeHooks> hooks = std::nullopt, bool hasGUI = false);
        void runSimulationWithNetwork(std::optional<flux::runtimeHooks> hooks, bool hasGUI, const std::string& serverIP, uint16_t serverPort);

    private:
#ifdef IS_CLIENT
        std::unique_ptr<client::network::TCPClient> _networkClient;

        void _setupNetwork(const std::string& serverIp, uint16_t serverPort);
#else
        void _setupNetwork(const std::string& serverIp, uint16_t serverPort) { return; };
#endif
};
