//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Struct used for network communication data
//

#pragma once

#include <cstdint>
#include <variant>
#include "utils/uuidGenerator.hpp"

namespace network
{
    constexpr std::uint8_t BUFFERSIZE = 64;

    struct ConnectionInfo;
    struct ClientTCPReceivedInfo;

    using NetworkData =
        std::variant<network::ClientTCPReceivedInfo, network::ConnectionInfo>;

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

    struct ClientTCPReceivedInfo final
    {
            bool ready;
            uint16_t portUDP;

            ClientTCPReceivedInfo() : ready(false), portUDP(0) {};
    };

    struct ClientTCPSentInfo final
    {
            char userID[BUFFERSIZE];
            uint16_t portUDP;
            uint16_t score;
    };

} // namespace network
