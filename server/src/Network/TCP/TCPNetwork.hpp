//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Class to handle the tcp connections
//

#pragma once

#include "Network/Network.hpp"
#include "network/datatype.hpp"
#include "Network/TCP/TCPInfo.hpp"
#include <boost/system/detail/error_code.hpp>
#include <boost/asio/ip/tcp.hpp>

using boost::asio::ip::tcp;

namespace network
{
    class TCPNetwork final: public Network
    {
        public:
            explicit TCPNetwork(uint16_t port);
            ~TCPNetwork();

            void connect();

        private:
            tcp::acceptor _acceptor;
            tcp::endpoint _endpoint;
            ClientTCPReceivedInfo _receivedInfo;
            std::vector<std::unique_ptr<ClientTCP>> _clients;

            void _setupAcceptNewSocket();
            void _acceptHandler(const boost::system::error_code& error);

            void _setupReadSocket(ClientTCP &client);
    };
} // namespace network
