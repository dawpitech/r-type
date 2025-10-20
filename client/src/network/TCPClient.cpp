/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** TCPClient.cpp
*/

#include <cstdint>
#include <functional>
#include <boost/system/error_code.hpp>

#include "network/TCPClient.hpp"
#include "network/datatype.hpp"
#include "utils/logger.hpp"

client::network::TCPClient::TCPClient(const std::string& serverIp, uint16_t serverPort, uint16_t selfUDPPort) :
    Network(serverIp, serverPort), _socket(this->_ioContext), _connected(false), _selfUDPPort(selfUDPPort)
{
    utils::Logger::debug(std::format("TCP Client created for {}:{}", serverIp, serverPort));
}

client::network::TCPClient::~TCPClient()
{
    if (this->_connected) {
        disconnect();
    }
}

void client::network::TCPClient::run()
{
    this->_ioContext.poll_one();
}

void client::network::TCPClient::connect()
{
    if (this->_connected) {
        utils::Logger::debug("Client already connected");
        return;
    }

    utils::Logger::debug(std::format("Attempting to connect to {}:{}", this->_serverIp, this->_serverPort));
    tcp::resolver resolver(this->_ioContext);
    auto endpoints = resolver.resolve(this->_serverIp, std::to_string(this->_serverPort));
    
    boost::asio::async_connect(this->_socket, endpoints, std::bind(&TCPClient::_connectHandler, this, std::placeholders::_1));
    
    this->_ioContext.poll_one();
}

void client::network::TCPClient::disconnect()
{
    if (!this->_connected) {
        return;
    }

    boost::system::error_code ec;
    this->_socket.shutdown(tcp::socket::shutdown_both, ec);
    this->_socket.close(ec);
    this->_connected = false;
    
}

void client::network::TCPClient::sendData(const ::network::ClientTCPReceivedInfo& data)
{
    if (!this->_connected) {
        return;
    }

    utils::Logger::debug("Client sent connection info");
    boost::asio::write(this->_socket, boost::asio::buffer(&data, sizeof(data)));
}

bool client::network::TCPClient::isConnected() const
{
    return this->_connected;
}

void client::network::TCPClient::_connectHandler(const boost::system::error_code& error)
{
    if (error) {
        utils::Logger::debug(std::format("Connection failed: {}", error.message()));
        throw NetworkError("Connection failed", "_connectHandler");
    }

    this->_connected = true;
    this->_setupRead();

}

void client::network::TCPClient::_setupRead()
{
    boost::system::error_code error;
    std::size_t bytesRead = boost::asio::read(
        this->_socket,
        boost::asio::buffer(&this->_info, sizeof(::network::ClientTCPSentInfo)),
        error
    );

    this->_readHandler(error, bytesRead);
}

void client::network::TCPClient::_readHandler(
    const boost::system::error_code& error,
    std::size_t bytesRead)
{
    if (error) {
        this->_connected = false;
        utils::Logger::debug(std::format("TCP read error: {}", error.message()));
        return;
    }

    if (bytesRead != sizeof(::network::ClientTCPSentInfo)) {
        utils::Logger::debug(std::format("Incomplete TCP packet ({} bytes)", bytesRead));
        return;
    }

    this->notify(this->_info);

    utils::Logger::debug(std::format("Client UUID: {}", this->_info.userID));

    ::network::ClientTCPReceivedInfo info;
    info.ready = true;
    std::strncpy(info.uuid, this->_info.userID, sizeof(info.uuid) - 1);
    utils::Logger::debug(std::format("uuid {}", info.uuid));
    info.portUDP = this->_selfUDPPort;

    try {
        boost::asio::write(this->_socket, boost::asio::buffer(&info, sizeof(info)));
        utils::Logger::debug(std::format("Sent ClientTCPReceivedInfo via TCP udpPort {}\n", info.portUDP));
    } catch (const boost::system::system_error& e) {
        utils::Logger::debug(std::format("Failed to send ClientTCPSReceivedInfo via TCP: {}", e.what()));
    }

    utils::Logger::debug(std::format("UDP client local port: {}\n", info.portUDP));
}
