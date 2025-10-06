/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** main.cpp
*/

#include <boost/program_options/errors.hpp>
#include <boost/program_options/variables_map.hpp>

#include <format>
#include <cstdlib>
#include <iostream>
#include <string>
#include "client/game.hpp"
#include "global/utils/error.hpp"
#include "global/utils/logger.hpp"
#include "client/parseArgs.hpp"

static void checkVariables(po::variables_map& variables)
{
    if (variables.count("ip") != 1)
    {
        throw utils::ParsingError("Arg ip undefined", "checkVariables");
    }
    if (variables.count("port") != 1)
    {
        throw utils::ParsingError("Arg port undefined", "checkVariables");
    }
    if (variables.find("debug") != variables.end())
    {
        utils::Logger::setDebug(true);
    }
}

int main(int argc, char **argv)
{
    try {
        rTypeClient::Game game;
        utils::Parser parser(argc, argv);
        parser.parseArgs();
        po::variables_map variables = parser.getArgs();

        checkVariables(variables);
        std::string msg =
            std::format("IP: {}", variables["ip"].as<std::string>());
        utils::Logger::debug(msg);
        msg = std::format("Port: {}", variables["port"].as<uint16_t>());
        utils::Logger::debug(msg);

        std::string ip = variables["ip"].as<std::string>();
        std::uint16_t port = variables["port"].as<uint16_t>();
        game.launchGame();
    }
    catch (const rTypeClient::GameError& e) {
        std::cerr << e.what() << "in" << e.where() << std::endl;
    }
    catch (const utils::BaseError& e) {
        std::cerr << e.what() << "in" << e.where() << std::endl;
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
