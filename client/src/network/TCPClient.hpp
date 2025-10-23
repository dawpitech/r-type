/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** TCPClient.hpp
*/

#pragma once

#include "Network.hpp"
#include "UDPClient.hpp"
#include "network/datatype.hpp"
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>
#include <cstdint>
#include <memory>

using boost::asio::ip::tcp;

namespace client::network {
    class TCPClient final : public Network {
       public:
        explicit TCPClient(
            const std::string &serverIp, uint16_t serverPort, uint16_t selfUdpPort, uint16_t selfVoicePort);
        ~TCPClient();

        void run();

        void waitConnection();
        void connect() override;
        void disconnect() override;

        void sendData(const ::network::ClientTCPReceivedInfo &data);
        bool isConnected() const;

       private:
        tcp::socket _socket;
        bool _connected;
        uint16_t _selfUDPPort;
        uint16_t _selfVoicePort;
        ::network::ClientTCPSentInfo _info;

        void _connectHandler(const boost::system::error_code &error);
        void _setupRead();
        void _readHandler(const boost::system::error_code &error, size_t bytesRead);
    };
}  // namespace client::network
