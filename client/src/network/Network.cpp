
#include <type_traits>
#include <iostream>
#include "client/network/Network.hpp"
#include "global/network/datatype.hpp"

client::network::Network::Network(const std::string& serverIp, uint16_t serverPort) : 
    _serverIp(serverIp), _serverPort(serverPort) {}

client::network::Network::~Network() {}

template <typename T> requires ::network::NetworkDataType<T>
void client::network::Network::attach(std::function<void(const T&)> callback)
{
    if constexpr(std::is_same_v<T, ::network::ClientTCPSentInfo>) {
        this->_tcpReceivedCallback = std::move(callback);
        return;
    }
    if constexpr(std::is_same_v<T, ::network::ConnectionInfo>) {
        this->_connectionCallback = std::move(callback);
        return;
    }
    if constexpr(std::is_same_v<T, ::network::UDPReceivedInfo>) {
        this->_udpReceivedCallback = std::move(callback);
        return;
    }
}

void client::network::Network::notify(const ::network::NetworkData& data)
{
    std::visit(
        [this](auto&& arg)
        {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, ::network::ClientTCPSentInfo>) {
                if (this->_tcpReceivedCallback)
                    this->_tcpReceivedCallback(arg);
                return;
            }
            if constexpr (std::is_same_v<T, ::network::UDPReceivedInfo>) {
                if (this->_udpReceivedCallback)
                    this->_udpReceivedCallback(arg);
                return;
            }
            if constexpr (std::is_same_v<T, ::network::ConnectionInfo>) {
                if (this->_connectionCallback)
                    this->_connectionCallback(arg);
                return;
            }
        },
        data);
}

template void client::network::Network::attach<::network::ClientTCPSentInfo>(
    std::function<void(const ::network::ClientTCPSentInfo&)>);
template void client::network::Network::attach<::network::UDPReceivedInfo>(
    std::function<void(const ::network::UDPReceivedInfo&)>);
template void client::network::Network::attach<::network::ConnectionInfo>(
    std::function<void(const ::network::ConnectionInfo&)>);
