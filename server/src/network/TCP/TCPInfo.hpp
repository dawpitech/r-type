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
#include <string>

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

            void sendTCPInfo(const ClientTCPSentInfo& data)
            {
                const ::network::PacketType header = ::network::PacketType::TCPInfo;
                boost::system::error_code ec;
                boost::asio::write(this->_socket, boost::asio::buffer(&header, sizeof(header)), ec);
                if (ec) {
                    utils::Logger::debug(std::format("TCP sendTCPInfo header error: {}", ec.message()));
                    return;
                }
                boost::asio::write(this->_socket, boost::asio::buffer(&data, sizeof(data)), ec);
                if (ec) {
                    utils::Logger::debug(std::format("TCP sendTCPInfo body error: {}", ec.message()));
                }
            }

            void sendChatReceive(const char senderId[::network::BUFFERSIZE], const ::network::ClientReceiveMessage &data)
            {
		if (!this->_socket.is_open()) return;
                const ::network::PacketType header = ::network::PacketType::ChatReceive;
                boost::system::error_code ec;
                boost::asio::write(this->_socket, boost::asio::buffer(&header, sizeof(header)), ec);
                if (ec) {
                    utils::Logger::debug(std::format("TCP sendChatReceive header error: {}", ec.message()));
                    return;
                }
                boost::asio::write(this->_socket, boost::asio::buffer(senderId, ::network::BUFFERSIZE), ec);
                if (ec) {
                    utils::Logger::debug(std::format("TCP sendChatReceive sender error: {}", ec.message()));
                    return;
                }
                boost::asio::write(this->_socket, boost::asio::buffer(&data, sizeof(data)), ec);
                if (ec) {
                    utils::Logger::debug(std::format("TCP sendChatReceive body error: {}", ec.message()));
                }
            }

            template <typename Handler>
            void acceptConnection(tcp::acceptor& acceptor, Handler&& handler)
            {
                acceptor.async_accept(this->_socket, std::forward<Handler>(handler));
            };

            template <typename ChatHandler>
            void async_read_with_chat(ServerNetwork& network, ChatHandler chatHandler)
            {
                auto header = std::make_shared<::network::PacketType>();
                boost::asio::async_read(
                    this->_socket,
                    boost::asio::buffer(header.get(), sizeof(::network::PacketType)),
                    [this, header, &network, chatHandler](const boost::system::error_code &error, size_t /*bytesRead*/)
                    {
                        if (error) {
                            utils::Logger::debug(std::format("Error in TCP read header: {}", error.message()));
                            return;
                        }

                        if (*header == ::network::PacketType::TCPInfo) {
                            auto data = std::make_shared<ClientTCPReceivedInfo>();
                            std::memset(data.get(), 0, sizeof(ClientTCPReceivedInfo));
                            boost::asio::async_read(
                                this->_socket, boost::asio::buffer(data.get(), sizeof(ClientTCPReceivedInfo)),
                                [this, data, &network, chatHandler](const boost::system::error_code &err, size_t) {
                                    if (err) {
                                        utils::Logger::debug(std::format("Error in TCP read body: {}", err.message()));
                                        return;
                                    }
                                    network.notify(*data);
                                    this->async_read_with_chat(network, chatHandler);
                                });
                        } else if (*header == ::network::PacketType::ChatSend) {
                            auto chat = std::make_shared<::network::ClientSendMessage>();
                            boost::asio::async_read(
                                this->_socket, boost::asio::buffer(chat.get(), sizeof(::network::ClientSendMessage)),
                                [this, chat, &network, chatHandler](const boost::system::error_code &err, size_t) {
                                    if (err) {
                                        utils::Logger::debug(std::format("Error in TCP read chat: {}", err.message()));
                                        return;
                                    }
                                    chatHandler(*chat, this->_socket);
                                    this->async_read_with_chat(network, chatHandler);
                                });
                        } else {
                            // Unknown packet, continue
                            this->async_read_with_chat(network, chatHandler);
                        }
                    });
            }

            [[nodiscard]] tcp::socket& getSocket() { return this->_socket; };

        private:
            tcp::socket _socket;
            ClientTCPReceivedInfo _data;
    };

} // namespace network
