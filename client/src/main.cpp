/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** main.cpp
*/

#include <boost/program_options/variables_map.hpp>
#include <cstdlib>
#include <flux/core/flux.hpp>
#include <format>
#include <iostream>
#include <sdlManager.hpp>

#include "parseArgs.hpp"
#include "Simulation.hpp"
#include "utils/error.hpp"
#include "utils/logger.hpp"

static void checkVariables(const po::variables_map& variables)
{
    if (!variables.contains("ip"))
    {
        throw utils::ParsingError("Arg ip undefined", "checkVariables");
    }
    if (!variables.contains("port"))
    {
        throw utils::ParsingError("Arg port undefined", "checkVariables");
    }
    if (variables.contains("debug"))
    {
        utils::Logger::setDebug(true);
    }
}

int main(int argc, char **argv)
{
    try {
        utils::Parser parser(argc, argv);
        parser.parseArgs();
        po::variables_map variables = parser.getArgs();

        checkVariables(variables);
        std::string msg =
            std::format("IP: {}", variables["ip"].as<std::string>());
        utils::Logger::debug(msg);
        msg = std::format("Port: {}", variables["port"].as<uint16_t>());
        utils::Logger::debug(msg);

        auto ip = variables["ip"].as<std::string>();
        std::uint16_t port = variables["port"].as<uint16_t>();

        flux::runtimeHooks hooks = {
            .hookBeforeRender = [] { render::SDLManager::clear(); },
            .hookAfterRender = [] { render::SDLManager::render(); },
        };

        Simulation simulation;
        simulation.runClientSimulation(hooks, ip, port);
    }
    catch (const utils::BaseError& e) {
        std::cerr << e.what() << " in " << e.where() << std::endl;
    }
    catch (const po::error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
