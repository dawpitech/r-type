//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Struct used for network communication data
//

#pragma once

#include <cstdint>
#include <variant>
#include "components/PlayerInput.hpp"
#include "utils/uuidGenerator.hpp"

namespace network
{
    constexpr uint16_t BUFFERSIZE = 64;

    enum class PacketType : uint8_t {
        TCPInfo = 1,
        ChatSend = 2,
        ChatReceive = 3,
    };

    struct ConnectionInfo;
    struct ClientTCPReceivedInfo;
    struct ClientTCPSentInfo;
    struct UDPReceivedInfo;
    struct UDPSentInfo;
    struct ClientSendMessage;
    struct ClientReceiveMessage;

    using NetworkData = std::variant<network::ClientTCPReceivedInfo, network::ClientTCPSentInfo, network::ConnectionInfo, network::UDPReceivedInfo, network::UDPSentInfo, network::ClientSendMessage, network::ClientReceiveMessage>;

    template <typename T>
    concept NetworkDataType = requires { std::get<T>(std::declval<NetworkData>()); };

    struct ConnectionInfo final
    {
            std::string ip;
            uint16_t port;
            std::string uuid;

            ConnectionInfo(std::string ip, uint16_t port) :
                ip(std::move(ip)), port(port), uuid(utils::UuidGenerator::generateUuid()) {};
    };

    struct ClientSendMessage final
    {
            char msg[BUFFERSIZE] = {};
    };

    struct ClientReceiveMessage final
    {
            char msg[BUFFERSIZE] = {};
    };

    struct ClientTCPReceivedInfo final
    {
            bool ready = false;
            char uuid[BUFFERSIZE] = {};
            uint16_t portUDP = 0;
    };

    struct ClientTCPSentInfo final
    {
            char userID[BUFFERSIZE];
            uint16_t portUDP;
            uint16_t score;

            ClientTCPSentInfo() : portUDP(0), score(0) {
                std::memset(this->userID, '\0', BUFFERSIZE);
            }

            explicit ClientTCPSentInfo(const std::string& id, uint16_t port, uint16_t score) : portUDP(port), score(score)
            {
                std::strcpy(this->userID, id.c_str());
            };
    };

    struct UDPReceivedInfo final
    {
            unsigned inputIndex = 0;
            char uuid[BUFFERSIZE] = "";
            component::PlayerInput game;
    };
    
    struct UDPSentInfo final
    {
        std::string serializedData;
    };

} // namespace network
