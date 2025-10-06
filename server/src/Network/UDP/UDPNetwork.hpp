//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Class to handle the udp network
//

#pragma once

#include <boost/asio.hpp>
#include "Network/Network.hpp"
#include "network/datatype.hpp"

using boost::asio::ip::udp;

namespace network
{
    class UDPNetwork final : public Network
    {
        public:
            explicit UDPNetwork(uint16_t port);
            ~UDPNetwork() = default;

        private:
            udp::endpoint _endpoint;
            udp::endpoint _remoteEndpoint;
            std::unique_ptr<udp::socket> _socket = nullptr;
            UDPReceivedInfo _data;

            void async_read();
    };
} // namespace network
