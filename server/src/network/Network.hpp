//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Class for network connection
//

#pragma once

#include <boost/asio.hpp>
#include <functional>

#include "network/datatype.hpp"
#include "utils/error.hpp"

namespace network {
    class NetworkError final : public utils::BaseError {
       public:
        NetworkError(const std::string &what, const std::string &where) : BaseError(what, where) {}
    };

    class ServerNetwork {
       public:
        explicit ServerNetwork(uint16_t port);
        ~ServerNetwork();

        virtual void connect();

        template <typename T>
        requires network::NetworkDataType<T> void attach(const std::function<void(const T &)> &&callback)
        {
            if constexpr (std::is_same_v<T, ClientTCPReceivedInfo>) {
                this->_tcpReceivedCallback = std::move(callback);
                return;
            }
            if constexpr (std::is_same_v<T, ConnectionInfo>) {
                this->_connectionCallback = std::move(callback);
                return;
            }
            if constexpr (std::is_same_v<T, UDPReceivedInfo>) {
                this->_udpReceivedCallback = std::move(callback);
                return;
            }
        }

        void notify(const NetworkData &data);

       protected:
        uint16_t _port;
        boost::asio::io_context _ioContext;

        std::function<void(const ClientTCPReceivedInfo &)> _tcpReceivedCallback;
        std::function<void(const UDPReceivedInfo &)> _udpReceivedCallback;
        std::function<void(const ConnectionInfo &)> _connectionCallback;

        std::string _compressString(const std::string &string);
        std::string _decompressString(const std::string &string);
    };
}  // namespace network
