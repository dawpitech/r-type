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

#include "systems/inputSystem.hpp"
#include "systems/movementSystem.hpp"
#include "systems/mobSystem.hpp"
#include "systems/mobShootSystem.hpp"
#include "systems/shootSystem.hpp"
#include "systems/projectileSystem.hpp"
#include "systems/collisionSystem.hpp"
#include "systems/damageSystem.hpp"
#include "systems/healthSystem.hpp"
#include "systems/inputDetectorSystem.hpp"
#include "systems/renderSystem.hpp"
#include "flux/core/flux.hpp"
#include "raylib-cpp.hpp"

#include "Simulation.hpp"
#include "parseArgs.hpp"
#include "utils/error.hpp"
#include "utils/logger.hpp"

#include <components/NetworkIdentification.hpp>
#include <components/Sprite.hpp>
#include <components/Transform.hpp>
#include <components/Velocity.hpp>

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

        raylib::Window window(800, 450, "R-Type");

        flux::runtimeHooks hooks = {
            .hookBeforeRender = [&window] {
                window.ClearBackground();
                BeginDrawing();
            },
            .hookAfterRender = [] {
                EndDrawing();
            },
        };

        flux::ECS ecs;

        Simulation::setInitialSimState(ecs);

        ecs.registerSystem(InputHandlerSystem, InputHandlerSystemView(ecs), flux::systemType::LOGIC);
        ecs.registerSystem(MovementSystem, MovementSystemView(ecs), flux::systemType::LOGIC);
        ecs.registerSystem(InputDetectorSystem, InputDetectorSystemView(ecs), flux::systemType::LOGIC);
        //ecs.registerSystem(MobSystem, MobSystemView(ecs), flux::systemType::LOGIC);
        //ecs.registerSystem(MobShootSystem, MobShootSystemView(ecs), flux::systemType::LOGIC);
        //ecs.registerSystem(ShootSystem, ShootSystemView(ecs), flux::systemType::LOGIC);
        //ecs.registerSystem(ProjectileSystem, ProjectileSystemView(ecs), flux::systemType::LOGIC);
        //ecs.registerSystem(CollisionSystem, CollisionSystemView(ecs), flux::systemType::LOGIC);
        //ecs.registerSystem(DamageSystem, DamageSystemView(ecs), flux::systemType::LOGIC);
        //ecs.registerSystem(HealthSystem, HealthSystemView(ecs), flux::systemType::LOGIC);
        //ecs.registerSystem(BackgroundSystem, BackgroundSystemView(ecs), flux::systemType::RENDER);
        //ecs.registerSystem(AnimationSystem, AnimationSystemView(ecs), flux::systemType::RENDER);
        ecs.registerSystem(RenderSystem, RenderSystemView(ecs), flux::systemType::RENDER);

        ecs.handExecution(hooks);
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
    catch (...) {
        std::cerr << "Unexpected Error" << std::endl;
    }
    return EXIT_SUCCESS;
}
