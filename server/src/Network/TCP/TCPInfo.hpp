//
// EPITECH PROJECT, 2025
// r-type
// File description:
// TCP data transmission struct
//

#pragma once

#include <cstdint>
#include "libs/asio/include/boost/asio/ip/tcp.hpp"

using boost::asio::ip::tcp;

namespace network
{
    constexpr std::uint8_t BUFFERSIZE = 64;

    class ClientTCP
    {
        public:
        private:
            std::unique_ptr<tcp::socket> _socket;
    };

    struct ClientTCPReceivedInfo
    {
            bool ready;
            uint16_t portUDP;
    };

    struct ClientTCPSentInfo
    {
            char userID[BUFFERSIZE];
            uint16_t portUDP;
    };
} // namespace network
