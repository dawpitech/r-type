//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Entrypoint for r-type server
//
//
#include <boost/program_options/errors.hpp>
#include <boost/program_options/variables_map.hpp>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include "Rooms/RoomsPool.hpp"
#include "logger.hpp"
#include "parseArgs.hpp"

static void checkVariables(po::variables_map& variables)
{
    if (variables.count("port") != 1)
    {
        throw utils::ParsingError("Arg port undefined", "checkVariables");
    }
    if (variables.count("rooms") != 1)
    {
        throw utils::ParsingError("Arg rooms undefined", "checkVariables");
    }
    if (variables.find("debug") != variables.end())
    {
        utils::Logger::setDebug(true);
    }
}

int main(int argc, char** argv)
{
    try
    {
        utils::Parser parser(argc, argv);
        parser.parseArgs();
        po::variables_map variables = parser.getArgs();

        checkVariables(variables);
        std::string msg =
            std::format("Port: {}", variables["port"].as<uint16_t>());
        utils::Logger::debug(msg);
        msg = std::format("Rooms: {}", variables["rooms"].as<uint16_t>());
        utils::Logger::debug(msg);

        std::uint16_t port = variables["port"].as<uint16_t>();
        std::uint16_t rooms = variables["rooms"].as<uint16_t>();
        Room::RoomsPool roomsPool(port, rooms);
        roomsPool.run();
    }
    catch (const utils::BaseError& e)
    {
        std::cerr << "Error in " << e.where() << ": " << e.what() << std::endl;
    }
    catch (const po::error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
