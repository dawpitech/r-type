/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** main.cpp
*/

#include <boost/program_options/variables_map.hpp>
#include <cstdlib>
#include <format>
#include <iostream>
#include <filesystem>

#include "parseArgs.hpp"
#include "utils/error.hpp"
#include "utils/logger.hpp"

#include <LuaContext.hpp>
#include <components/Sprite.hpp>

#include "Client.hpp"

static void checkVariables(const po::variables_map &variables)
{
    if (!variables.contains("ip")) {
        throw utils::ParsingError("Arg ip undefined", "checkVariables");
    }
    if (!variables.contains("port")) {
        throw utils::ParsingError("Arg port undefined", "checkVariables");
    }
    if (variables.contains("debug")) {
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

        const auto serverIP = variables["ip"].as<std::string>();
        const auto serverPort = variables["port"].as<uint16_t>();

        client::Client gameClient(serverIP, serverPort);

        gameClient.run();
    } catch (const utils::BaseError &e) {
        std::cerr << e.what() << " in " << e.where() << std::endl;
    } catch (const po::error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unexpected Error" << std::endl;
    }
    return EXIT_SUCCESS;
}
