//
// EPITECH PROJECT, 2025
// r-type
// File description:
// 
//

#pragma once


#include <cstdint>

namespace network {
    constexpr std::uint8_t BUFFERSIZE = 64;

    struct ClientTCPReceivedInfo
    {
        int port;
        uint8_t status;
    };

    struct ClientTCPSentInfo
    {
        char userID[BUFFERSIZE];
    };
}
