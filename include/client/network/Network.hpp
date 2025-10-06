
#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <utility>
#include <variant>
#include "global/utils/error.hpp"
#include "global/network/datatype.hpp"

namespace client::network
{
    class NetworkError final : public utils::BaseError
    {
        public:
            NetworkError(const std::string& what, const std::string& where) : BaseError(what, where) {}
    };

    class Network
    {
        public:
            explicit Network(const std::string& serverIp, uint16_t serverPort);
            ~Network();

            template <typename T>
                requires ::network::NetworkDataType<T>
            void attach(std::function<void(const T&)> callback);
            void notify(const ::network::NetworkData& data);

            virtual void connect() = 0;
            virtual void disconnect() = 0;

        protected:
            std::string _serverIp;
            uint16_t _serverPort;
            boost::asio::io_context _ioContext;

            std::function<void(const ::network::ClientTCPReceivedInfo&)> _tcpReceivedCallback;
            std::function<void(const ::network::UDPReceivedInfo&)> _udpReceivedCallback;
            std::function<void(const ::network::ConnectionInfo&)> _connectionCallback;
    };
} // namespace client::network
