//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Class to handle the udp network
//

#pragma once

#include "Network/Network.hpp"

namespace network
{
    class UDPNetwork : public Network
    {
        public:
            explicit UDPNetwork(uint16_t port);
            ~UDPNetwork();

        private:
    };
} // namespace network
