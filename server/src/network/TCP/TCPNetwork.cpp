//
// EPITECH PROJECT, 2025
// r-type
// File description:
// Function for the tcp connection
//

#include <functional>
#include <boost/system/error_code.hpp>
#include <boost/uuid/uuid.hpp>

#include "../../../../client/src/network/Network.hpp"
#include "network/TCP/TCPInfo.hpp"
#include "TCPNetwork.hpp"

network::TCPNetwork::TCPNetwork(const uint16_t port) :
    ServerNetwork(port), _acceptor(this->_ioContext), _endpoint(tcp::v4(), port)
{
    this->_acceptor.open(this->_endpoint.protocol());
    this->_acceptor.set_option(tcp::acceptor::reuse_address(true));
    this->_acceptor.bind(this->_endpoint);
    this->_acceptor.listen();

    this->_setupAcceptNewSocket();
}

network::TCPNetwork::~TCPNetwork() {}

void network::TCPNetwork::_setupAcceptNewSocket()
{
    this->_clients.emplace_back(std::make_unique<ClientTCP>(this->_ioContext));
    if (this->_clients.back() == nullptr) {
        throw NetworkError("Unable to create new client", "TCP accept");
    }
    this->_clients.back()->acceptConnection(this->_acceptor,
                                            std::bind(&TCPNetwork::_acceptHandler, this, std::placeholders::_1));
}

void network::TCPNetwork::_acceptHandler(const boost::system::error_code& error)
{
    if (error) {
        utils::Logger::debug(std::format("Error in TCP accept: {}", error.message()));
        return;
    }
    auto &client = this->_clients.back();
    this->_setupReadSocket(*client);

    auto& tcpSocket = client->getSocket();
    auto socketIp = tcpSocket.remote_endpoint().address().to_string();
    auto socketPort = tcpSocket.remote_endpoint().port();
    std::string msg = std::format("New connection accepted from: {}:{}", socketIp, socketPort);

    ConnectionInfo info(socketIp, socketPort);
    this->notify(info);

    ClientTCPSentInfo dataToSend(info.uuid, this->_port, 0);
    client->send(dataToSend);
    utils::Logger::debug(msg);
    this->_setupAcceptNewSocket();
}

void network::TCPNetwork::_setupReadSocket(network::ClientTCP& client) { client.async_read(*this); }
