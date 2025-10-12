//
// EPITECH PROJECT, 2025
// r-type
// File description:
// Functions for the udp connection
//

#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/system/detail/error_code.hpp>
#include <memory>

#include "UDPNetwork.hpp"
#include "network/datatype.hpp"
#include "utils/logger.hpp"

network::UDPNetwork::UDPNetwork(std::uint16_t port) : ServerNetwork(port), _endpoint(udp::v4(), port)
{
    this->_socket = std::make_unique<udp::socket>(this->_ioContext, this->_endpoint);
    this->async_read();
}

void network::UDPNetwork::sendData(std::string &ip, uint16_t port, const std::string &data)
{
    udp::endpoint remoteEndpoint(boost::asio::ip::make_address(ip), port);
    this->_socket->send_to(boost::asio::buffer(data), remoteEndpoint);
}

void network::UDPNetwork::async_read()
{
    auto data = std::make_shared<UDPReceivedInfo>();
    auto remoteEndpoint = std::make_shared<udp::endpoint>();

    this->_socket->async_receive_from(
        boost::asio::buffer(data.get(), sizeof(network::UDPReceivedInfo)), 
        *remoteEndpoint,
        [this, data, remoteEndpoint](const boost::system::error_code& error, size_t bytesRead)
        {
            if (error) {
                utils::Logger::debug(std::format("Error in UDP read: {}", error.message()));
                this->async_read();
                return;
            }

            if (bytesRead != sizeof(UDPReceivedInfo)) {
                utils::Logger::debug(std::format("Error in UDP read size\nexpected: {}\nbut got: {}",
                                                 sizeof(UDPReceivedInfo), bytesRead));
                this->async_read();
                return;
            }
            this->notify(*data);
            this->async_read();
        });
}
