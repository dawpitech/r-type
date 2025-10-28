/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** Network.cpp
*/

#include <iostream>
#include <type_traits>
#include <typeinfo>

#include <zlib.h>
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
    if constexpr (std::is_same_v<T, ::network::ClientReceiveMessage>) {
        this->_chatReceiveCallback = std::move(callback);
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
                if (this->_tcpSentCallback)
                    this->_tcpSentCallback(arg);
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
            if constexpr (std::is_same_v<T, ::network::UDPSentInfo>) {
                if (this->_udpSentCallback) {
                    this->_udpSentCallback(arg);
                }
                return;
            }
            if constexpr (std::is_same_v<T, ::network::ClientReceiveMessage>) {
                if (this->_chatReceiveCallback) {
                    this->_chatReceiveCallback(arg);
                }
                return;
            }
        },
        data);
}

std::string client::network::Network::_compressString(const std::string &str)
{
    auto destLen = compressBound(str.length());
    std::string compressedData(destLen, '\0');

    int res = compress2(reinterpret_cast<Bytef *>(compressedData.data()), &destLen,
        reinterpret_cast<const Bytef *>(str.c_str()), str.length(), Z_BEST_COMPRESSION);

    if (res != Z_OK)
        throw NetworkError("Unable to compress data", "Server");
    compressedData.resize(destLen);
    return compressedData;
}

std::string client::network::Network::_decompressString(const std::string &compressedStr)
{
    auto bufferSize = compressedStr.length() * 2;
    std::string decompressedData;
    int res = Z_BUF_ERROR;
    auto destLen = bufferSize;

    while (res == Z_BUF_ERROR) {
        decompressedData.resize(bufferSize);
        destLen = bufferSize;

#ifndef _WIN32
        res = uncompress(
            reinterpret_cast<Bytef*>(decompressedData.data()),
            &destLen,
            reinterpret_cast<const Bytef*>(compressedStr.c_str()),
            compressedStr.length()
        );
#else
        res = uncompress(
            reinterpret_cast<Bytef*>(decompressedData.data()),
            reinterpret_cast<unsigned long*>(&destLen),
            reinterpret_cast<const Bytef*>(compressedStr.c_str()),
            compressedStr.length()
        );
#endif
        if (res == Z_BUF_ERROR)
            bufferSize *= 2;
    }

    if (res != Z_OK)
        throw NetworkError("Unable to decompress data", "Server");
    decompressedData.resize(destLen);
    return decompressedData;
}

template void client::network::Network::attach<::network::ClientTCPSentInfo>(
    std::function<void(const ::network::ClientTCPSentInfo&)>);
template void client::network::Network::attach<::network::UDPReceivedInfo>(
    std::function<void(const ::network::UDPReceivedInfo&)>);
template void
    client::network::Network::attach<::network::ConnectionInfo>(std::function<void(const ::network::ConnectionInfo&)>);
template void
    client::network::Network::attach<::network::UDPSentInfo>(std::function<void(const ::network::UDPSentInfo&)>);
template void client::network::Network::attach<::network::ClientReceiveMessage>(
    std::function<void(const ::network::ClientReceiveMessage&)>);
