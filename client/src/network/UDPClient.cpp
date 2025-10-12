/*
** EPITECH PROJECT, 2025
** UDPClient
** File description:
** udp
*/

#include "UDPClient.hpp"
#include <memory>
#include "network/datatype.hpp"
#include "utils/logger.hpp"

client::network::UDPClient::UDPClient(const std::string& serverIp, uint16_t serverPort) :
    _serverEndpoint(boost::asio::ip::make_address(serverIp), serverPort), client::network::Network(serverIp, serverPort)
{
    this->_socket = std::make_unique<udp::socket>(this->_ioContext, udp::endpoint(udp::v4(), 0));
    this->async_read();
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
    auto buffer = std::make_shared<std::vector<char>>(8192);
    auto remoteEndpoint = std::make_shared<udp::endpoint>();

    this->_socket->async_receive_from(
        boost::asio::buffer(*buffer),
        *remoteEndpoint,
        [this, buffer, endpoint = remoteEndpoint](const boost::system::error_code& error, size_t bytesRead)
        {
            if (error) {
                utils::Logger::debug(std::format("Error in UDP read: {}", error.message()));
                this->async_read();
                return;
            }
            
            ::network::UDPSentInfo info;
            info.serializedData = std::string(buffer->begin(), buffer->begin() + bytesRead);
            
            this->notify(info);
            this->async_read();
        });
}

void client::network::UDPClient::connect() { this->_ioContext.poll_one(); }
