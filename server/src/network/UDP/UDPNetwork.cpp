//
// EPITECH PROJECT, 2025
// r-type
// File description:
// Functions for the udp connection
//

#include <boost/asio/buffer.hpp>
#include <boost/system/detail/error_code.hpp>
#include <memory>

#include "UDPNetwork.hpp"
#include "network/datatype.hpp"
#include "utils/logger.hpp"

network::UDPNetwork::UDPNetwork(std::uint16_t port) : Network(port), _endpoint(udp::v4(), port)
{
    this->_socket = std::make_unique<udp::socket>(this->_ioContext, this->_endpoint);
    this->async_read();
}

void network::UDPNetwork::async_read()
{
    auto data = std::make_unique<UDPReceivedInfo>();
    auto remoteEndpoint = std::make_unique<udp::endpoint>();

    this->_socket->async_receive_from(
        boost::asio::buffer(&this->_data, sizeof(network::UDPReceivedInfo)), this->_remoteEndpoint,
        [this, data = std::move(data), endpoint = std::move(remoteEndpoint)](const boost::system::error_code& error,
                                                                             size_t bytesRead) mutable
        {
            this->async_read();
            if (error) {
                utils::Logger::debug(std::format("Error in UDP read: {}", error.message()));
                return;
            }

            if (bytesRead != sizeof(UDPReceivedInfo)) {
                utils::Logger::debug(std::format("Error in UDP read size\nexpected: {}\nbut got: {}",
                                                 sizeof(UDPReceivedInfo), bytesRead));
                return;
            }
            this->notify(this->_data);
        });
}
