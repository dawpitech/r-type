/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Simulation.cpp
*/

#ifdef IS_CLIENT
#include "network/datatype.hpp"
#include "network/TCPClient.hpp"
#include "network/UDPClient.hpp"
#endif

#include "components/NetworkIdentification.hpp"
#include "components/Collider.hpp"
#include "components/Health.hpp"
#include "components/Mob.hpp"
#include "components/Player.hpp"
#include "components/PlayerInput.hpp"
#include "components/Projectile.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"
#include "flux/core/Serialization.hpp"
#include "Simulation.hpp"

void Simulation::setInitialSimState(flux::ECS& ecs)
{
    _registerComponent(ecs);
    _createEntities(ecs);
}

void Simulation::_registerComponent(flux::ECS& ecs)
{
    ecs.registerComponentType<component::collider>("Collider");
    ecs.registerComponentType<component::Health>("Health");
    ecs.registerComponentType<component::mob>("Mob");
    ecs.registerComponentType<component::Player>("Player");
    ecs.registerComponentType<component::PlayerInput>("PlayerInput");
    ecs.registerComponentType<component::Projectile>("Projectile");
    ecs.registerComponentType<component::sprite>("Sprite");
    ecs.registerComponentType<component::Transform>("Transform");
    ecs.registerComponentType<component::Velocity>("Velocity");
    ecs.registerComponentType<component::NetworkIdentification>("NetworkIdentification");
    ecs.Register<component::collider>();
    ecs.Register<component::Health>();
    ecs.Register<component::mob>();
    ecs.Register<component::Player>();
    ecs.Register<component::PlayerInput>();
    ecs.Register<component::Projectile>();
    ecs.Register<component::sprite>();
    ecs.Register<component::Transform>();
    ecs.Register<component::Velocity>();
    ecs.Register<component::NetworkIdentification>();
}

void Simulation::_createEntities(flux::ECS& ecs)
{
    _createPlayer(ecs, PLAYER_TYPE::PLAYER_ONE);

    //_createMob(ecs, utils::Vector2(255, 150));
    //_createMob(ecs, utils::Vector2(1900, 300));

    const flux::Entity background = ecs.newEntity();
    ecs.Add<component::sprite>(background, component::sprite("./assets/starfield2.jpg"));
    ecs.Add<component::Transform>(background, component::Transform(0, 0, 0, 1, 1));
}

void Simulation::_createPlayer(flux::ECS& ecs, PLAYER_TYPE type)
{
    constexpr int startX = 0;
    constexpr int width = 66;
    constexpr int height = 34;
    int startY;

    switch (type) {
        case PLAYER_TYPE::PLAYER_ONE:
            startY = 0;
            break;
        case PLAYER_TYPE::PLAYER_TWO:
            startY = 34;
            break;
        case PLAYER_TYPE::PLAYER_THREE:
            startY = 68;
            break;
        default:
        case PLAYER_TYPE::PLAYER_FOUR:
            startY = 102;
            break;
    }

    const flux::Entity playerEntity = ecs.newEntity();
    ecs.Add<component::sprite>(playerEntity, component::sprite("./assets/player.gif", startX, startY, width, height));
    ecs.Add<component::Player>(playerEntity);
    ecs.Add<component::PlayerInput>(playerEntity);
    ecs.Add<component::NetworkIdentification>(playerEntity);
    ecs.Add<component::Transform>(playerEntity, component::Transform(0, 0, 0, 1, 1));
    ecs.Add<component::Velocity>(playerEntity, component::Velocity());
    ecs.Add<component::Health>(playerEntity);
    /*
    ecs.Add<component::collider>(
        playerEntity,
        component::collider(component::CollisionLayer::PLAYER,
                            component::CollisionLayer::MOB | component::CollisionLayer::MOB_PROJECTILE, 0, 0,
                            playerSprite.frameSize.x, playerSprite.frameSize.y));
    */
}

void Simulation::_createMob(flux::ECS& ecs, const utils::Vector2<int>& pos)
{
    /*
    const flux::Entity mobEntity = ecs.newEntity();
    const render::SpriteData& mobSprite = render::SDLManager::load("./assets/mob1.gif");

    ecs.Add<component::mob>(mobEntity, component::mob(10, 0, true, 0.0f, 2.0f));
    ecs.Add<component::sprite>(mobEntity, component::sprite(mobSprite.texture));
    ecs.Add<component::animation>(mobEntity, component::animation(mobSprite.spriteMap, true));
    ecs.Add<component::Transform>(mobEntity, component::Transform(static_cast<float>(pos.x), static_cast<float>(pos.y), 0, 1, 1));
    ecs.Add<component::Velocity>(mobEntity);
    ecs.Add<component::collider>(
        mobEntity,
        component::collider(component::CollisionLayer::MOB,
                            component::CollisionLayer::PLAYER | component::CollisionLayer::PLAYER_PROJECTILE, 0, 0,
                            mobSprite.frameSize.x, mobSprite.frameSize.y));
    ecs.Add<component::Health>(mobEntity, component::Health(40));
    */
}

/*
void Simulation::_setupNetwork(const std::string& serverIp, uint16_t serverPort,
                               std::optional<flux::runtimeHooks>& hooks)
{
    utils::Logger::debug(std::format("Setting up network connection to {}:{}", serverIp, serverPort));
    this->_networkUDPClient = std::make_unique<client::network::UDPClient>(serverIp, serverPort);
    this->_networkTCPClient =
        std::make_unique<client::network::TCPClient>(serverIp, serverPort, this->_networkUDPClient->getLocalPort());

    this->_networkTCPClient->attach<network::ClientTCPSentInfo>([this](const network::ClientTCPSentInfo& info)
                                                                { this->gameInfo = info; });
    this->_networkUDPClient->attach<network::UDPSentInfo>(
        [this](const network::UDPSentInfo& info)
        {
            this->_ecs.unserializeAllComponents(info.serializedData);
        });
    hooks->hooksNetwork = [this](flux::ECS& ecs) { this->_networkUDPClient->connect(); };

    this->_lastInputSend = std::chrono::steady_clock::now();

    hooks->hookPlayerInput = [this](flux::ECS& ecs)
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->_lastInputSend);

        if (elapsed.count() < 50) {
            return;
        }

        std::unordered_map<flux::Entity, std::vector<std::any>> componentStore;
        ecs.getEntities<component::NetworkIdentification>(ecs, componentStore);
        ecs.getEntities<component::PlayerInput>(ecs, componentStore);

        for (const auto& [entity, components] : componentStore) {
            const component::NetworkIdentification* netId = nullptr;
            const component::PlayerInput* playerInput = nullptr;

            for (const auto& component : components) {
                if (component.type() == typeid(component::NetworkIdentification)) {
                    netId = std::any_cast<component::NetworkIdentification>(&component);
                }
                if (component.type() == typeid(component::PlayerInput)) {
                    playerInput = std::any_cast<component::PlayerInput>(&component);
                }
            }

            if (netId && playerInput) {
                if (std::strcmp(netId->uuid, this->gameInfo.userID) == 0) {
                    network::UDPReceivedInfo data;
                    std::strcpy(data.uuid, this->gameInfo.userID);
                    data.game = *playerInput;

                    static component::PlayerInput lastSentInput;
                    if (lastSentInput != data.game) {
                        this->_networkUDPClient->async_write(data);
                        lastSentInput = data.game;
                        this->_lastInputSend = now;
                    }
                    continue;
                }
            }
        }
    };
    try {
        this->_networkTCPClient->connect();
        utils::Logger::debug("Network setup completed");
    }
    catch (const client::network::NetworkError& e) {
        utils::Logger::debug(std::format("Network connection failed: {}", e.what()));
        throw utils::BaseError("Failed to connect to server", "_setupNetwork");
    }
}
*/
