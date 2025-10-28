/*
** EPITECH PROJECT, 2025
** UDPClient
** File description:
** udp
*/

#pragma once

#include "Network.hpp"
#include "network/datatype.hpp"
#include "utils/logger.hpp"
#include <boost/asio.hpp>
#include <memory>

using boost::asio::ip::udp;

namespace client::network {
    template <typename T>
    class UDPClient final : public client::network::Network {
       public:
        explicit UDPClient(const std::string &serverIp, uint16_t serverPort)
            : _serverEndpoint(boost::asio::ip::make_address(serverIp), serverPort),
              client::network::Network(serverIp, serverPort)
        {
            this->_socket = std::make_unique<udp::socket>(this->_ioContext, udp::endpoint(udp::v4(), 0));
            if (this->_socket == nullptr)
                throw NetworkError("Unable to create socket", "udp client");
            this->async_read();
        }

        ~UDPClient() = default;

        void async_write(const ::network::UDPReceivedInfo &data)
        {
            auto dataPtr = std::make_shared<::network::UDPReceivedInfo>(data);

            this->_socket->async_send_to(
                boost::asio::buffer(dataPtr.get(), sizeof(::network::UDPReceivedInfo)), _serverEndpoint,
                [dataPtr](const boost::system::error_code &error, std::size_t bytesSent) {
                    if (error) {
                        utils::Logger::debug(std::format("Error in UDP write: {}", error.message()));
                        return;
                    }

                    if (bytesSent != sizeof(::network::UDPReceivedInfo)) {
                        utils::Logger::debug(
                            std::format("Error in UDP write size\nexpected: {}\nbut sent: {}",
                                sizeof(::network::UDPReceivedInfo), bytesSent));
                        return;
                    }

                    utils::Logger::debug(std::format("Successfully sent UDP packet of {} bytes", bytesSent));
                });
        }

        void async_write(const ::network::UDPVoiceInfo &data)
        {
            auto dataPtr = std::make_shared<::network::UDPVoiceInfo>(data);

            this->_socket->async_send_to(boost::asio::buffer(dataPtr.get(), sizeof(::network::UDPVoiceInfo)),
                _serverEndpoint, [dataPtr](const boost::system::error_code &error, std::size_t bytesSent) {
                    if (error) {
                        utils::Logger::debug(std::format("Error in UDP voice write: {}", error.message()));
                        return;
                    }

                    if (bytesSent != sizeof(::network::UDPVoiceInfo)) {
                        utils::Logger::debug(
                            std::format("Error in UDP voice write size\nexpected: {}\nbut sent: {}",
                                sizeof(::network::UDPVoiceInfo), bytesSent));
                        return;
                    }

                    utils::Logger::debug(
                        std::format("Successfully sent voice UDP packet of {} bytes", bytesSent));
                });
        }

        void async_read()
        {
            auto buffer = std::make_shared<std::vector<char>>(8192);
            auto remoteEndpoint = std::make_shared<udp::endpoint>();

            this->_socket->async_receive_from(boost::asio::buffer(*buffer), *remoteEndpoint,
                [this, buffer, endpoint = remoteEndpoint](
                    const boost::system::error_code &error, size_t bytesRead) {
                    if (error) {
                        utils::Logger::debug(std::format("Error in UDP read: {}", error.message()));
                        this->async_read();
                        return;
                    }

                    T info;
                    auto decompressedData =
                        this->_decompressString(std::string(buffer->begin(), buffer->begin() + bytesRead));
                    info.serializedData = decompressedData;

                    this->notify(info);
                    this->async_read();
                });
        }

        void connect() override { this->_ioContext.poll_one(); }

        uint16_t getLocalPort() const { return _socket ? _socket->local_endpoint().port() : 0; }

       private:
        std::unique_ptr<udp::socket> _socket = nullptr;
        udp::endpoint _serverEndpoint;
        std::string _data;

        void disconnect() override
        {
            if (_socket && _socket->is_open())
                _socket->close();
        }
    };
}  // namespace client::network
