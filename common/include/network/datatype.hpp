//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Struct used for network communication data
//

#pragma once

#include "components/PlayerInput.hpp"
#include "utils/uuidGenerator.hpp"
#include <cstdint>
#include <variant>

namespace network {
    constexpr uint16_t BUFFERSIZE = 64;

    struct ConnectionInfo;
    struct ClientTCPReceivedInfo;
    struct ClientTCPSentInfo;
    struct UDPReceivedInfo;
    struct UDPSentInfo;
    struct UDPVoiceInfo;

    using NetworkData = std::variant<network::ClientTCPReceivedInfo, network::ClientTCPSentInfo,
        network::ConnectionInfo, network::UDPReceivedInfo, network::UDPSentInfo, network::UDPVoiceInfo>;

    template <typename T>
    concept NetworkDataType = requires
    {
        std::get<T>(std::declval<NetworkData>());
    };

    struct ConnectionInfo final {
        std::string ip;
        uint16_t port;
        std::string uuid;

        ConnectionInfo(std::string ip, uint16_t port)
            : ip(std::move(ip)), port(port), uuid(utils::UuidGenerator::generateUuid()) {};
    };

    struct ClientTCPReceivedInfo final {
        bool ready = false;
        char uuid[BUFFERSIZE] = {};
        uint16_t portUDP = 0;
        uint16_t portVoiceChat = 0;
    };

    struct ClientTCPSentInfo final {
        char userID[BUFFERSIZE];
        uint16_t portUDP;
        uint16_t score;

        ClientTCPSentInfo() : portUDP(0), score(0) { std::memset(this->userID, '\0', BUFFERSIZE); }

        explicit ClientTCPSentInfo(const std::string &id, uint16_t port, uint16_t score)
            : portUDP(port), score(score)
        {
            std::strcpy(this->userID, id.c_str());
        };
    };

    struct UDPReceivedInfo final {
        unsigned inputIndex = 0;
        char uuid[BUFFERSIZE] = "";
        // std::string soundBuffer = "";
        component::PlayerInput game;
    };

    struct UDPSentInfo final {
        std::string serializedData;
    };

    struct UDPVoiceInfo final {
        std::string userID;
        std::string soundBuffer;
    };

}  // namespace network
