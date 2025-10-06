
#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>
#include "Network.hpp"
#include "global/network/datatype.hpp"

using boost::asio::ip::tcp;

namespace client::network
{
    class TCPClient final : public Network
    {
        public:
            explicit TCPClient(const std::string& serverIp, uint16_t serverPort);
            ~TCPClient();

            void connect() override;
            void disconnect() override;
            
            void sendData(const ::network::ClientTCPReceivedInfo& data);
            bool isConnected() const;

        private:
            tcp::socket _socket;
            bool _connected;

            void _connectHandler(const boost::system::error_code& error);
            void _setupRead();
            void _readHandler(const boost::system::error_code& error, size_t bytesRead, std::unique_ptr<::network::ClientTCPSentInfo>&& data);
    };
} // namespace client::network
