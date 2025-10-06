//
// EPITECH PROJECT, 2025
// r-type
// File description:
// TCP data transmission struct
//

#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/uuid/uuid.hpp>
#include <utility>

#include "network/Network.hpp"
#include "network/datatype.hpp"

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

            template <typename Handler>
            void async_read(Network& network, Handler&& handler)
            {
                this->_socket.async_receive(
                    boost::asio::buffer(&this->_data, sizeof(ClientTCPReceivedInfo)),
                    [this, &network, handler](const boost::system::error_code& error, size_t bytesRead)
                    {
                        if (error) {
                            handler(error, bytesRead);
                            return;
                        }

                        if (bytesRead == sizeof(ClientTCPReceivedInfo))
                            this->addData(network, this->_data);

                        handler(error, bytesRead);
                        this->async_read(network, handler);
                    });
            }

            [[nodiscard]] tcp::socket& getSocket() { return this->_socket; };

            void addData(Network& network, const ClientTCPReceivedInfo& data) { network.notify(data); }

        private:
            tcp::socket _socket;
            ClientTCPReceivedInfo _data;
    };

} // namespace network
