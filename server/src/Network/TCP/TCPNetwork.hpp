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

            void connect();

        private:
            tcp::acceptor _acceptor;
            tcp::endpoint _endpoint;
            std::vector<std::unique_ptr<tcp::socket>> _sockets;

            void _acceptHandler(const boost::system::error_code& error);
            void _acceptNewSocket();
    };
} // namespace network
