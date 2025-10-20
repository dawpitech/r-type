//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Base function for the network class
//

#include <type_traits>
#include <zlib.h>

#include "Network.hpp"
#include "network/datatype.hpp"

network::ServerNetwork::ServerNetwork(const uint16_t port) : _port(port) {}

network::ServerNetwork::~ServerNetwork() {}

void network::ServerNetwork::connect()
{
    this->_ioContext.poll_one();
}

void network::ServerNetwork::notify(const NetworkData &data)
{
    std::visit(
        [this](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, ClientTCPReceivedInfo>) {
                if (this->_tcpReceivedCallback)
                    this->_tcpReceivedCallback(arg);
                return;
            }
            if constexpr (std::is_same_v<T, UDPReceivedInfo>) {
                if (this->_udpReceivedCallback)
                    this->_udpReceivedCallback(arg);
                return;
            }
            if constexpr (std::is_same_v<T, ConnectionInfo>) {
                if (this->_connectionCallback)
                    this->_connectionCallback(arg);
                return;
            }
        },
        data);
}

std::string network::ServerNetwork::_compressString(const std::string &str)
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

std::string network::ServerNetwork::_decompressString(const std::string &compressedStr)
{
    auto bufferSize = compressedStr.length() * 2;
    std::string decompressedData;
    int res = Z_BUF_ERROR;
    auto destLen = bufferSize;

    while (res == Z_BUF_ERROR) {
        decompressedData.resize(bufferSize);
        destLen = bufferSize;

        res = uncompress(
            reinterpret_cast<Bytef*>(decompressedData.data()),
            &destLen,
            reinterpret_cast<const Bytef*>(compressedStr.c_str()),
            compressedStr.length()
        );
        if (res == Z_BUF_ERROR)
            bufferSize *= 2;
    }

    if (res != Z_OK)
        throw NetworkError("Unable to decompress data", "Server");
    decompressedData.resize(destLen);
    return decompressedData;
}
