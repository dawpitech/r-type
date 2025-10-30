//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Function to run client main class
//

#include "Client.hpp"
#include <Camera2D.hpp>
#include <chrono>
#include <functional>
#include <memory>
#include <raylib.h>
#include <unordered_map>
#include <Window.hpp>
#include <vector>
#include "CameraRaylib.hpp"
#include "components/Animation.hpp"
#include "components/NetworkIdentification.hpp"
#include "flux/core/Serialization.hpp"
#include "network/datatype.hpp"
#include "Simulation.hpp"
#include "systems/animationSystem.hpp"
#include "systems/inputDetectorSystem.hpp"
#include "systems/renderSystem.hpp"
#include "systems/setCamera.hpp"
#include "systems/endGameSystem.hpp"
#include "utils/logger.hpp"

client::Client::Client(const std::string& ip, uint16_t port) : _ip(ip), _port(port)
{
    this->_window = std::make_unique<raylib::Window>(WINDOW_BASE_WIDTH, WINDOW_BASE_HEIGHT, WINDOW_BASE_NAME);
    this->_camera = std::make_unique<raylib::Camera2D>();
    this->_networkUDPClient = std::make_unique<client::network::UDPClient>(ip, port);
    this->_networkTCPClient = std::make_unique<client::network::TCPClient>(ip, port, _networkUDPClient->getLocalPort());

    Simulation::setInitialClientSimState(this->_ecs, "Menu");
    this->_registerBase();
    this->_initHooks();
}

void client::Client::_registerBase()
{
    this->_ecs.Register<component::Animation>();

    this->_ecs.registerSystem(InputDetectorSystem, InputDetectorSystemView(this->_ecs), flux::systemType::LOGIC);
    this->_ecs.registerSystem(RenderSystem, RenderSystemView(this->_ecs), flux::systemType::RENDER);
    this->_ecs.registerSystem(AnimationSystem, AnimationSystemView(this->_ecs), flux::systemType::RENDER);
    this->_ecs.registerSystem(setCameraSystem, setCameraSystemView(this->_ecs), flux::systemType::LOGIC);
    this->_ecs.registerSystem(endGameSystem, endGameSystemView(this->_ecs), flux::systemType::RENDER);
}

void client::Client::_initHooks()
{
    auto& window = this->_window;
    auto& masterRunVar = this->_ecs.getMasterRunState();

    this->_hooks = {
        .hookBeforeRender =
            [this, &window, &masterRunVar]
        {
            window->ClearBackground();
            BeginDrawing();
            BeginMode2D(CameraRaylib::getCamera());
            if (window->ShouldClose())
                masterRunVar = false;
            this->_handleChatInput();
        },
        .hookAfterRender = [this] { EndMode2D(); this->_renderChat(); EndDrawing(); },
    };
    this->_initNetworkableHooks();
}

void client::Client::_initNetworkableHooks()
{
    utils::Logger::debug(std::format("Setting up network connection to {}:{}", this->_ip, this->_port));

    this->_networkTCPClient->attach<::network::ClientTCPSentInfo>([this](const ::network::ClientTCPSentInfo& info)
                                                                  { this->_gameInfo = info; });
    this->_networkUDPClient->attach<::network::UDPSentInfo>(
        [this](const ::network::UDPSentInfo& info)
        {
            if (this->_gameLaunch == false) {
                Simulation::setInitialClientSimState(this->_ecs, "Level_0");
                this->_registerBase();
                this->_initHooks();
            }
            this->_gameLaunch = true;
            this->_ecs.unserializeAllComponents(info.serializedData);
        });
    this->_networkTCPClient->attach<::network::ClientReceiveMessage>([this](const ::network::ClientReceiveMessage &msg) {
        this->_chatLog.emplace_back(std::string(msg.msg), msg.hexcol);
        if (this->_chatLog.size() > 6)
            this->_chatLog.pop_front();
    });
    this->_hooks.hooksNetwork = [this](flux::ECS&)
    {
        this->_networkTCPClient->run();
        this->_networkUDPClient->connect();
    };

    this->_lastInputSend = std::chrono::steady_clock::now();

    this->_hooks.hookPlayerInput = [this](flux::ECS& ecs)
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->_lastInputSend);

        if (this->_chatTyping || elapsed.count() < INPUT_SEND_DELAY)
            return;

        std::unordered_map<flux::Entity, std::vector<std::any>> componentStore;
        ecs.getEntities<component::NetworkIdentification>(ecs, componentStore);
        ecs.getEntities<component::PlayerInput>(ecs, componentStore);

        this->_sendPlayerInput(componentStore, now);
    };
}

void client::Client::_sendPlayerInput(const std::unordered_map<flux::Entity, std::vector<std::any>>& componentStore,
                                      const std::chrono::steady_clock::time_point& now)
{
    for (const auto& [entity, components] : componentStore) {
        const component::NetworkIdentification* netId = nullptr;
        const component::PlayerInput* playerInput = nullptr;

        for (const auto& component : components) {
            if (component.type() == typeid(component::NetworkIdentification))
                netId = std::any_cast<component::NetworkIdentification>(&component);
            if (component.type() == typeid(component::PlayerInput))
                playerInput = std::any_cast<component::PlayerInput>(&component);
        }

        if (netId && playerInput) {
            if (std::strcmp(netId->uuid, this->_gameInfo.userID) == 0) {
                ::network::UDPReceivedInfo data;
                std::strcpy(data.uuid, this->_gameInfo.userID);
                data.game = *playerInput;

                if (this->_lastSentInput != data.game) {
                    data.inputIndex = this->_inputIndex;
                    this->_inputIndex += 1;
                    this->_inputs.push(data.game);
                    for (size_t i = 0; i < NB_REDUNDANCY; i += 1) {
                        this->_networkUDPClient->async_write(data);
                    }
                    this->_lastSentInput = data.game;
                    this->_lastInputSend = now;
                }
            }
        }
    }
}

void client::Client::run()
{
    try {
        this->_networkTCPClient->connect();
        utils::Logger::debug("Network setup completed");
    }
    catch (const client::network::NetworkError& e) {
        utils::Logger::debug(std::format("Network connection failed: {}", e.what()));
        throw utils::BaseError("Failed to connect to server", "_setupNetwork");
    }
    this->_ecs.handExecution(this->_hooks);
}

void client::Client::_handleChatInput()
{
    if (IsKeyPressed(KEY_ENTER)) {
        if (!this->_chatTyping) {
            this->_chatTyping = true;
            this->_chatBuffer.clear();
        } else {
            this->_chatTyping = false;
            if (!this->_chatBuffer.empty()) {
                ::network::ClientSendMessage msg{};
                std::snprintf(msg.msg, ::network::BUFFERSIZE, "%s", this->_chatBuffer.c_str());
                this->_networkTCPClient->sendChat(msg);
            }
        }
    }
    if (this->_chatTyping) {
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 125) {
                if (this->_chatBuffer.size() < (::network::BUFFERSIZE - 1))
                    this->_chatBuffer.push_back(static_cast<char>(key));
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !this->_chatBuffer.empty())
            this->_chatBuffer.pop_back();
    }
}

void client::Client::_renderChat()
{
    BeginScissorMode(0, WINDOW_BASE_HEIGHT - 120, 400, 120);
    int y = WINDOW_BASE_HEIGHT - 40;
    for (auto it = this->_chatLog.rbegin(); it != this->_chatLog.rend(); ++it) {
        raylib::Color col = raylib::Color(it->second);
        col.DrawText(it->first.c_str(), 10, y, 16);
        y -= 18;
        if (y < WINDOW_BASE_HEIGHT - 120)
            break;
    }
    if (this->_chatTyping) {
        std::string line = "> " + this->_chatBuffer + ((GetTime() - static_cast<int>(GetTime()) < 0.5) ? "_" : "");
        DrawText(line.c_str(), 10, WINDOW_BASE_HEIGHT - 18, 16, YELLOW);
    }
    EndScissorMode();
}
