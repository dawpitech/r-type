//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Entrypoint for r-type server
//

#include <cstdlib>
#include <exception>
#include <format>
#include <iostream>
#include <boost/program_options/errors.hpp>
#include <boost/program_options/variables_map.hpp>

#include "Server.hpp"
#include "parser/parseArgs.hpp"
#include "utils/logger.hpp"

static void checkVariables(const po::variables_map& variables)
{
    if (!variables.contains("port"))
    {
        throw utils::ParsingError("Arg port undefined", "checkVariables");
    }
    if (!variables.contains("rooms"))
    {
        throw utils::ParsingError("Arg rooms undefined", "checkVariables");
    }
    if (variables.contains("debug") && !variables.contains("cli"))
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
        bool cli = variables.contains("cli");
        Server::Server roomsPool(port, rooms, cli);
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
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unexpected Error" << std::endl;
    }

    return EXIT_SUCCESS;
}
