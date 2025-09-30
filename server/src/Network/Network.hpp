//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Class for network connection
//

#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <utility>
#include <variant>
#include "utils/error.hpp"

namespace network
{
    struct ClientTCPReceivedInfo;
    struct ConnectionInfo;

    using NetworkData =
        std::variant<network::ClientTCPReceivedInfo, network::ConnectionInfo>;

    template <typename T>
    concept NetworkDataType = requires { std::get<T>(std::declval<NetworkData>()); };

    class NetworkError final : public utils::BaseError
    {
        public:
            NetworkError(const std::string& what, const std::string& where) : BaseError(what, where) {}
    };

    class Network
    {
        public:
            explicit Network(uint16_t port);
            ~Network();

            template <typename T>
                requires NetworkDataType<T>
            void attach(std::function<void(const T&)> callback);
            void notify(const NetworkData& data);

        protected:
            uint16_t _port;
            boost::asio::io_context _io_context;

            std::function<void(const ClientTCPReceivedInfo&)> _tcpReceivedCallback;
            std::function<void(const ConnectionInfo&)> _connectionCallback;
    };
} // namespace network
