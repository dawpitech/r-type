//
// EPITECH PROJECT, 2025
// r-type
// File description:
// Function for the tcp connection
//

#include "TCPNetwork.hpp"
#include <iostream>
#include "Network/Network.hpp"
#include "libs/system/include/boost/system/detail/error_code.hpp"

network::TCPNetwork::TCPNetwork(const uint16_t port) :
    Network(port), _acceptor(this->_io_context),
    _endpoint(tcp::v4(), port)
{
    this->_acceptor.open(this->_endpoint.protocol());
    this->_acceptor.set_option(tcp::acceptor::reuse_address(true));
    this->_acceptor.bind(this->_endpoint);
    this->_acceptor.listen();
    this->_socket = std::make_unique<tcp::socket>(this->_io_context);

    if (this->_socket == nullptr) {
        throw NetworkError("Unable to create socket", "TCP init");
    }
    this->_acceptor.async_accept(*this->_socket,
       network::TCPNetwork::_acceptHandler);
}

network::TCPNetwork::~TCPNetwork() {}

void network::TCPNetwork::_acceptHandler(const boost::system::error_code& error)
{
    if (error)
    {
        std::cerr << "Error: " << error.message() << std::endl;
        return;
    }
    std::cout << "New connection accepted" << std::endl;
}
