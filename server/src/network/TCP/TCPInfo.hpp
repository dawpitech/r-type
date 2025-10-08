//
// EPITECH PROJECT, 2025
// r-type
// File description:
// TCP data transmission struct
//

#pragma once

#include <utility>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "network/Network.hpp"
#include "network/datatype.hpp"
#include "utils/logger.hpp"

using boost::asio::ip::tcp;

namespace network
{
    class ClientTCP final
    {
        public:
            explicit ClientTCP(boost::asio::io_context& io_context) : _socket(io_context) {};
            ~ClientTCP() = default;

            template <typename Handler>
            void acceptConnection(tcp::acceptor& acceptor, Handler&& handler)
            {
                acceptor.async_accept(this->_socket, std::forward<Handler>(handler));
            };

            void async_read(ServerNetwork& network)
            {
                boost::asio::async_read(
                    this->_socket, boost::asio::buffer(&this->_data, sizeof(ClientTCPReceivedInfo)),
                    [this, &network](const boost::system::error_code& error, size_t bytesRead)
                    {
                        if (error) {
                            utils::Logger::debug(std::format("Error in TCP read: {}", error.message()));
                            return;
                        }

                        if (bytesRead != sizeof(ClientTCPReceivedInfo)) {
                            utils::Logger::debug(std::format("Error in TCP read size\nexpected: {}\nbut got: {}",
                                                             sizeof(ClientTCPReceivedInfo), bytesRead));
                            return;
                        }
                        network.notify(this->_data);
                        this->async_read(network);
                    });
            }

            [[nodiscard]] tcp::socket& getSocket() { return this->_socket; };

            void addData(ServerNetwork& network, const ClientTCPReceivedInfo& data) { network.notify(data); }

        private:
            tcp::socket _socket;
            ClientTCPReceivedInfo _data;
    };

} // namespace network
