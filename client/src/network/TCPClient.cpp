
#include <functional>
#include <boost/system/error_code.hpp>
#include "client/network/TCPClient.hpp"
#include "global/utils/logger.hpp"

client::network::TCPClient::TCPClient(const std::string& serverIp, uint16_t serverPort) :
    Network(serverIp, serverPort), _socket(this->_ioContext), _connected(false)
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
    
    boost::asio::async_connect(this->_socket, endpoints,
        std::bind(&TCPClient::_connectHandler, this, std::placeholders::_1));
    
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

void client::network::TCPClient::_connectHandler(const boost::system::error_code& error)
{
    if (error) {
        utils::Logger::debug(std::format("Connection failed: {}", error.message()));
        throw NetworkError("Connection failed", "_connectHandler");
    }

    this->_connected = true;
    utils::Logger::debug(std::format("Successfully connected to {}:{}", this->_serverIp, this->_serverPort));
    this->_setupRead();
}

void client::network::TCPClient::_setupRead()
{
    auto data = std::make_unique<::network::ClientTCPSentInfo>();
    boost::asio::async_read(
        this->_socket, 
        boost::asio::buffer(data.get(), sizeof(::network::ClientTCPSentInfo)),
        [this, data = std::move(data)](const boost::system::error_code& error, size_t bytesRead) mutable {
            this->_readHandler(error, bytesRead, std::move(data));
        });
}

void client::network::TCPClient::_readHandler(const boost::system::error_code& error, size_t bytesRead, std::unique_ptr<::network::ClientTCPSentInfo>&& data)
{
    if (error) {
        this->_connected = false;
        return;
    }

    if (bytesRead != sizeof(::network::ClientTCPSentInfo)) {
        return;
    }

    this->notify(*data);
    this->_setupRead();
}
