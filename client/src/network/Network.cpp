/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** Network.cpp
*/

#include <iostream>
#include <type_traits>
#include <typeinfo>

#include "Network.hpp"
#include "network/datatype.hpp"

client::network::Network::Network(const std::string& serverIp, uint16_t serverPort) :
    _serverIp(serverIp), _serverPort(serverPort)
{}

client::network::Network::~Network() {}

template <typename T>
    requires ::network::NetworkDataType<T>
void client::network::Network::attach(std::function<void(const T&)> callback)
{
    if constexpr (std::is_same_v<T, ::network::ClientTCPSentInfo>) {
        this->_tcpSentCallback = std::move(callback);
        return;
    }
    if constexpr (std::is_same_v<T, ::network::ConnectionInfo>) {
        this->_connectionCallback = std::move(callback);
        return;
    }
    if constexpr (std::is_same_v<T, ::network::UDPReceivedInfo>) {
        this->_udpReceivedCallback = std::move(callback);
        return;
    }
    if constexpr (std::is_same_v<T, ::network::UDPSentInfo>) {
        this->_udpSentCallback = std::move(callback);
        return;
    }
}

void client::network::Network::notify(const ::network::NetworkData& data)
{
    // std::cout << "Inside the notify" << std::endl;
    std::visit(
        [this](auto&& arg)
        {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, ::network::ClientTCPSentInfo>) {
                if (this->_tcpSentCallback)
                    this->_tcpSentCallback(arg);
                return;
            }
            if constexpr (std::is_same_v<T, ::network::UDPReceivedInfo>) {
                std::cout << "This is the UDPReceivedInfo" << std::endl;
                if (this->_udpReceivedCallback)
                    this->_udpReceivedCallback(arg);
                return;
            }
            if constexpr (std::is_same_v<T, ::network::ConnectionInfo>) {
                if (this->_connectionCallback)
                    this->_connectionCallback(arg);
                return;
            }
            if constexpr (std::is_same_v<T, ::network::UDPSentInfo>) {
                if (this->_udpSentCallback) {
                    this->_udpSentCallback(arg);
                }
                return;
            }
        },
        data);
}

template void client::network::Network::attach<::network::ClientTCPSentInfo>(
    std::function<void(const ::network::ClientTCPSentInfo&)>);
template void client::network::Network::attach<::network::UDPReceivedInfo>(
    std::function<void(const ::network::UDPReceivedInfo&)>);
template void
    client::network::Network::attach<::network::ConnectionInfo>(std::function<void(const ::network::ConnectionInfo&)>);
template void
    client::network::Network::attach<::network::UDPSentInfo>(std::function<void(const ::network::UDPSentInfo&)>);
