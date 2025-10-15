//
// EPITECH PROJECT, 2025
// r-type
// File description:
// TCP data transmission struct
//

#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <utility>

#include "network/datatype.hpp"
#include "network/Network.hpp"
#include "utils/logger.hpp"

using boost::asio::ip::tcp;

namespace network
{
    class ClientTCP final
    {
        public:
            explicit ClientTCP(boost::asio::io_context& io_context) : _socket(io_context) {};
            ~ClientTCP() = default;

            void send(const ClientTCPSentInfo& data)
            {
                boost::asio::write(this->_socket, boost::asio::buffer(&data, sizeof(data)));
            }

            template <typename Handler>
            void acceptConnection(tcp::acceptor& acceptor, Handler&& handler)
            {
                acceptor.async_accept(this->_socket, std::forward<Handler>(handler));
            };

            void async_read(ServerNetwork& network)
            {
                auto data = std::make_unique<ClientTCPReceivedInfo>();
                auto* dataPtr = data.get();
                std::memset(dataPtr, 0, sizeof(ClientTCPReceivedInfo));

                boost::asio::async_read(
                    this->_socket, boost::asio::buffer(dataPtr, sizeof(ClientTCPReceivedInfo)),
                    [this, data = std::move(data), &network](const boost::system::error_code& error, size_t bytesRead)
                    {
                        if (error) {
                            utils::Logger::debug(std::format("Error in TCP read: {}", error.message()));
                            return;
                        }

                        const unsigned char* bytes = reinterpret_cast<const unsigned char*>(data.get());

                        network.notify(*data);
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
