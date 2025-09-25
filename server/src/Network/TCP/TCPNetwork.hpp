//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Class to handle the tcp connections
//

#pragma once

#include "Network/Network.hpp"
#include "libs/asio/include/boost/asio/ip/tcp.hpp"
#include "libs/system/include/boost/system/detail/error_code.hpp"

using boost::asio::ip::tcp;

namespace network
{
    class TCPNetwork : public Network
    {
        public:
            explicit TCPNetwork(uint16_t port);
            ~TCPNetwork();

        private:
            tcp::acceptor _acceptor;
            tcp::endpoint _endpoint;
            std::unique_ptr<tcp::socket> _socket = nullptr;

            static void _acceptHandler(const boost::system::error_code &error);
    };
} // namespace network
