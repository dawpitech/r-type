/*
** EPITECH PROJECT, 2025
** UDPClient
** File description:
** udp
*/

#include "UDPClient.hpp"
#include "utils/logger.hpp"
#include <memory>

client::network::UDPClient::UDPClient(const std::string &serverIp, uint16_t serverPort)
    : _serverEndpoint(boost::asio::ip::make_address(serverIp), serverPort), client::network::Network(serverIp, serverPort)
{
    this->_socket = std::make_unique<udp::socket>(this->_ioContext, udp::endpoint(udp::v4(), 0));
}

void client::network::UDPClient::async_write(const ::network::UDPReceivedInfo& data)
{
    auto dataPtr = std::make_shared<::network::UDPReceivedInfo>(data);

    this->_socket->async_send_to(
        boost::asio::buffer(dataPtr.get(), sizeof(::network::UDPReceivedInfo)), _serverEndpoint,
        [dataPtr](const boost::system::error_code& error, std::size_t bytesSent)
        {
            if (error) {
                utils::Logger::debug(std::format("Error in UDP write: {}", error.message()));
                return;
            }

            if (bytesSent != sizeof(::network::UDPReceivedInfo)) {
                utils::Logger::debug(std::format("Error in UDP write size\nexpected: {}\nbut sent: {}",
                                                 sizeof(::network::UDPReceivedInfo), bytesSent));
                return;
            }

            utils::Logger::debug(std::format("Successfully sent UDP packet of {} bytes", bytesSent));
        });
}

void client::network::UDPClient::async_read()
{
}
