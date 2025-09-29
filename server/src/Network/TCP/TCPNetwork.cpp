//
// EPITECH PROJECT, 2025
// r-type
// File description:
// Function for the tcp connection
//

#include <functional>
#include "TCPNetwork.hpp"
#include "Network/Network.hpp"
#include "libs/system/include/boost/system/detail/error_code.hpp"
#include "utils/logger.hpp"

network::TCPNetwork::TCPNetwork(const uint16_t port) :
    Network(port), _acceptor(this->_io_context), _endpoint(tcp::v4(), port)
{
    this->_acceptor.open(this->_endpoint.protocol());
    this->_acceptor.set_option(tcp::acceptor::reuse_address(true));
    this->_acceptor.bind(this->_endpoint);
    this->_acceptor.listen();

    this->_acceptNewSocket();
}

network::TCPNetwork::~TCPNetwork() {}

void network::TCPNetwork::connect()
{
    int hasRun = this->_io_context.poll_one();
}

void network::TCPNetwork::_acceptNewSocket()
{
    this->_sockets.emplace_back(
        std::make_unique<tcp::socket>(this->_io_context));
    if (this->_sockets.back() == nullptr)
    {
        throw NetworkError("Unable to create socket", "TCP accept");
    }
    this->_acceptor.async_accept(
        *this->_sockets.back(),
        std::bind(&TCPNetwork::_acceptHandler, this, std::placeholders::_1));
}

void network::TCPNetwork::_acceptHandler(const boost::system::error_code& error)
{
    if (error)
    {
        utils::Logger::debug(std::format("Error: {}", error.message()));
        return;
    }
    auto socketIp = this->_sockets.back()->remote_endpoint().address().to_string();
    auto socketPort = this->_sockets.back()->remote_endpoint().port();
    std::string msg = std::format("New connection accepted from: {}:{}", socketIp, socketPort);
    utils::Logger::debug(msg);
    this->_acceptNewSocket();
}
