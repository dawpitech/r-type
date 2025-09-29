//
// EPITECH PROJECT, 2025
// r-type
// File description:
// TCP data transmission struct
//

#pragma once

#include <cstdint>
#include <optional>
#include <queue>
#include <utility>
#include "libs/asio/include/boost/asio/io_context.hpp"
#include "libs/asio/include/boost/asio/ip/tcp.hpp"

using boost::asio::ip::tcp;

namespace network
{
    constexpr std::uint8_t BUFFERSIZE = 64;

    struct ClientTCPReceivedInfo
    {
            bool ready;
            uint16_t portUDP;

            ClientTCPReceivedInfo() : ready(false), portUDP(0) {};
    };

    struct ClientTCPSentInfo
    {
            char userID[BUFFERSIZE];
            uint16_t portUDP;
    };

    class ClientTCP
    {
        public:
            explicit ClientTCP(boost::asio::io_context& io_context) :
                _socket(io_context) {};
            ~ClientTCP() = default;

            template <typename Handler>
            void acceptConnection(tcp::acceptor& acceptor, Handler&& handler)
            {
                acceptor.async_accept(this->_socket,
                                      std::forward<Handler>(handler));
            };

            template <typename Handler>
            void async_read(Handler&& handler)
            {
                ClientTCPReceivedInfo info;

                this->_socket.async_receive(
                    boost::asio::buffer(&info, sizeof(info)), std::forward<Handler>(handler));
                this->_receivedInfos.push(info);
            }

            [[nodiscard]] tcp::socket& getSocket() { return this->_socket; };

            void addData(ClientTCPReceivedInfo data)
            {
                this->_receivedInfos.push(data);
            }

            std::optional<ClientTCPReceivedInfo> getData()
            {
                if (this->_receivedInfos.empty())
                    return std::nullopt;

                auto data = this->_receivedInfos.front();
                this->_receivedInfos.pop();
                return data;
            }

        private:
            tcp::socket _socket;
            std::queue<ClientTCPReceivedInfo> _receivedInfos;
    };

} // namespace network
