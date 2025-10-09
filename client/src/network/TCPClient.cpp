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

client::network::TCPClient::TCPClient(const std::string& serverIp, uint16_t serverPort) :
    Network(serverIp, serverPort), _socket(this->_ioContext), _connected(false), _uuid("")
{
    utils::Logger::debug(std::format("TCP Client created for {}:{}", serverIp, serverPort));
}

client::network::TCPClient::~TCPClient()
{
    if (this->_connected) {
        disconnect();
    }
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

    boost::asio::write(this->_socket, boost::asio::buffer(&data, sizeof(data)));
}

bool client::network::TCPClient::isConnected() const
{
    return this->_connected;
}

uint16_t client::network::TCPClient::getPortUDP() const
{
    return this->_portUDP;
}

void client::network::TCPClient::_connectHandler(const boost::system::error_code& error)
{
    if (error) {
        utils::Logger::debug(std::format("Connection failed: {}", error.message()));
        throw NetworkError("Connection failed", "_connectHandler");
    }

    this->_connected = true;
    utils::Logger::debug(std::format("Successfully connected to {}:{}", this->_serverIp, this->_serverPort));
    this->_setupRead();

    if (this->_uuid.empty()) {
        utils::Logger::debug("Error, client UUID was empty");
        return;
    }

    utils::Logger::debug(std::format("Client UUID: {}", this->_uuid));

    this->_networkClientUDP = std::make_unique<client::network::UDPClient>(
        this->_serverIp, this->_portUDP);

    ::network::ClientTCPReceivedInfo info;
    info.ready = true;
    std::strncpy(info.uuid, this->_uuid.c_str(), sizeof(info.uuid) - 1);
    info.uuid[sizeof(info.uuid) - 1] = '\0';
    utils::Logger::debug(std::format("uuid {}", info.uuid));
    info.portUDP = this->_networkClientUDP->getLocalPort();

    try {
        boost::asio::write(this->_socket, boost::asio::buffer(&info, sizeof(info)));
        utils::Logger::debug(std::format("Sent ClientTCPReceivedInfo via TCP ({} bytes)", sizeof(info)));
    } catch (const boost::system::system_error& e) {
        utils::Logger::debug(std::format("Failed to send ClientTCPSReceivedInfo via TCP: {}", e.what()));
    }

    utils::Logger::debug(std::format("UDP client local port: {}", info.portUDP));
}

void client::network::TCPClient::_setupRead()
{
    auto data = std::make_unique<::network::ClientTCPSentInfo>();

    boost::system::error_code error;
    std::size_t bytesRead = boost::asio::read(
        this->_socket,
        boost::asio::buffer(data.get(), sizeof(::network::ClientTCPSentInfo)),
        error
    );

    this->_readHandler(error, bytesRead, std::move(data));
}

void client::network::TCPClient::_readHandler(
    const boost::system::error_code& error,
    std::size_t bytesRead,
    std::unique_ptr<::network::ClientTCPSentInfo>&& data)
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

    this->notify(*data);

    this->_uuid    = data->userID;
    this->_portUDP = data->portUDP;
    this->_score   = data->score;
}
