#include <boost/program_options/errors.hpp>
#include <boost/program_options/variables_map.hpp>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include "Rooms/RoomsPool.hpp"
#include "parseArgs.hpp"

static void checkVariables(po::variables_map& variables)
{
    if (variables.count("port") != 1)
    {
        std::cerr << "No port defined" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (variables.count("rooms") != 1)
    {
        std::cerr << "No rooms defined" << std::endl;
        exit(EXIT_FAILURE);
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
        if (variables.find("debug") != variables.end())
        {
            std::cout << "Port: " << variables["port"].as<uint16_t>()
                      << std::endl;
            std::cout << "Rooms: " << variables["rooms"].as<uint16_t>()
                      << std::endl;
        }

        Room::RoomsPool roomsPool;
    }
    catch (const po::error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
