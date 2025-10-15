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

#include "flux/core/flux.hpp"
#include "raylib-cpp.hpp"
#include "systems/collisionSystem.hpp"
#include "systems/damageSystem.hpp"
#include "systems/healthSystem.hpp"
#include "systems/inputDetectorSystem.hpp"
#include "systems/inputSystem.hpp"
#include "systems/mobShootSystem.hpp"
#include "systems/mobSystem.hpp"
#include "systems/movementSystem.hpp"
#include "systems/projectileSystem.hpp"
#include "systems/renderSystem.hpp"
#include "systems/shootSystem.hpp"

#include "Simulation.hpp"
#include "network/TCPClient.hpp"
#include "network/UDPClient.hpp"
#include "parseArgs.hpp"
#include "utils/error.hpp"
#include "utils/logger.hpp"

#include <components/Animation.hpp>
#include <components/NetworkIdentification.hpp>
#include <components/Sprite.hpp>
#include <components/Transform.hpp>
#include <components/Velocity.hpp>
#include <systems/animationSystem.hpp>

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

        flux::ECS ecs;
        raylib::Window window(800, 450, "R-Type");
        auto &masterRunVar = ecs.getMasterRunState();

        flux::runtimeHooks hooks = {
            .hookBeforeRender =
                [&window, &masterRunVar] {
                    window.ClearBackground();
                    BeginDrawing();
                    if (window.ShouldClose())
                        masterRunVar = false;
                },
            .hookAfterRender = [] { EndDrawing(); },
        };

        Simulation::setInitialSimState(ecs);

        ecs.Register<component::Animation>();

        ecs.registerSystem(InputDetectorSystem, InputDetectorSystemView(ecs),
            flux::systemType::LOGIC);
        ecs.registerSystem(
            RenderSystem, RenderSystemView(ecs), flux::systemType::RENDER);
        ecs.registerSystem(AnimationSystem, AnimationSystemView(ecs),
            flux::systemType::RENDER);

        const auto projectileEntity = ecs.newEntity();
        ecs.Add<component::Transform>(
            projectileEntity, component::Transform(100, 100, 0, 1, 1));
        ecs.Add<component::Velocity>(projectileEntity);
        ecs.Add<component::Sprite>(projectileEntity,
            component::Sprite(
                "assets/player_shoot_spritesheet.png", 96, 0, 32, 32));
        ecs.Add<component::Animation>(projectileEntity,
            component::Animation(
                4, 200, "assets/player_shoot_spritesheet.png", true, 32, 32));
        std::unique_ptr<client::network::TCPClient> _networkTCPClient =
            nullptr;
        std::unique_ptr<client::network::UDPClient> _networkUDPClient =
            nullptr;
        _networkUDPClient =
            std::make_unique<client::network::UDPClient>(serverIP, serverPort);
        _networkTCPClient = std::make_unique<client::network::TCPClient>(
            serverIP, serverPort, _networkUDPClient->getLocalPort());

        network::ClientTCPSentInfo gameInfo;
        if constexpr (true) {
            utils::Logger::debug(
                std::format("Setting up network connection to {}:{}", serverIP,
                    serverPort));

            std::chrono::steady_clock::time_point _lastInputSend;

            _networkTCPClient->attach<network::ClientTCPSentInfo>(
                [&gameInfo](const network::ClientTCPSentInfo &info) {
                    gameInfo = info;
                });
            _networkUDPClient->attach<network::UDPSentInfo>(
                [&ecs](const network::UDPSentInfo &info) {
                    ecs.unserializeAllComponents(info.serializedData);
                });
            hooks.hooksNetwork = [&_networkUDPClient](flux::ECS &) {
                _networkUDPClient->connect();
            };

            _lastInputSend = std::chrono::steady_clock::now();

            hooks.hookPlayerInput = [&_lastInputSend, &gameInfo,
                                        &_networkUDPClient](flux::ECS &ecs) {
                auto now = std::chrono::steady_clock::now();
                auto elapsed =
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                        now - _lastInputSend);

                if (elapsed.count() < 20) {
                    return;
                }

                std::unordered_map<flux::Entity, std::vector<std::any>>
                    componentStore;
                ecs.getEntities<component::NetworkIdentification>(
                    ecs, componentStore);
                ecs.getEntities<component::PlayerInput>(ecs, componentStore);

                for (const auto &[entity, components] : componentStore) {
                    const component::NetworkIdentification *netId = nullptr;
                    const component::PlayerInput *playerInput = nullptr;

                    for (const auto &component : components) {
                        if (component.type() ==
                            typeid(component::NetworkIdentification)) {
                            netId = std::any_cast<
                                component::NetworkIdentification>(&component);
                        }
                        if (component.type() ==
                            typeid(component::PlayerInput)) {
                            playerInput =
                                std::any_cast<component::PlayerInput>(
                                    &component);
                        }
                    }

                    if (netId && playerInput) {
                        if (std::strcmp(netId->uuid, gameInfo.userID) == 0) {
                            network::UDPReceivedInfo data;
                            std::strcpy(data.uuid, gameInfo.userID);
                            data.game = *playerInput;

                            static component::PlayerInput lastSentInput;
                            if (lastSentInput != data.game) {
                                _networkUDPClient->async_write(data);
                                lastSentInput = data.game;
                                _lastInputSend = now;
                            }
                        }
                    }
                }
            };
            try {
                _networkTCPClient->connect();
                utils::Logger::debug("Network setup completed");
            } catch (const client::network::NetworkError &e) {
                utils::Logger::debug(
                    std::format("Network connection failed: {}", e.what()));
                throw utils::BaseError(
                    "Failed to connect to server", "_setupNetwork");
            }
        }

        ecs.handExecution(hooks);
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
