/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** TCPClient.hpp
*/

#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>
#include <cstdint>
#include <memory>
#include "Network.hpp"
#include "UDPClient.hpp"
#include "network/datatype.hpp"

using boost::asio::ip::tcp;

namespace client::network
{
    class TCPClient final : public Network
    {
        public:
            explicit TCPClient(const std::string& serverIp, uint16_t serverPort, uint16_t selfUdpPort);
            ~TCPClient();

            void connect() override;
            void disconnect() override;

            void sendData(const ::network::ClientTCPReceivedInfo& data);
            bool isConnected() const;
            uint16_t getPortUDP() const;

        private:
            tcp::socket _socket;
            bool _connected;
            std::string _uuid;
            uint16_t _portUDP;
            uint16_t _selfUDPPort;
            uint16_t _score = 0;

            void _connectHandler(const boost::system::error_code& error);
            void _setupRead();
            void _readHandler(const boost::system::error_code& error, size_t bytesRead, std::unique_ptr<::network::ClientTCPSentInfo>&& data);
    };
} // namespace client::network
