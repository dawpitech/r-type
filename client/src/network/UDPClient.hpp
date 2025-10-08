/*
** EPITECH PROJECT, 2025
** UDPClient
** File description:
** udp
*/

#pragma once

#include <boost/asio.hpp>
#include "network/datatype.hpp"
#include "Network.hpp"
#include <memory>

using boost::asio::ip::udp;

namespace client::network
{
    class UDPClient final : public client::network::Network
    {
    public:
        explicit UDPClient(const std::string &serverIp, uint16_t serverPort);
        ~UDPClient() = default;

        void async_write(const ::network::UDPReceivedInfo &data);
        void async_read();

        uint16_t getLocalPort() const { return _socket ? _socket->local_endpoint().port() : 0; }

    private:
        std::unique_ptr<udp::socket> _socket = nullptr;
        udp::endpoint _serverEndpoint;

        void connect() override {}
        void disconnect() override
        {
            if (_socket && _socket->is_open())
                _socket->close();
        }
    };
}
