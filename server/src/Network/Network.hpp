//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Class for network connection
//

#pragma once

#include <boost/asio.hpp>
#include "error.hpp"

namespace network
{

    class NetworkError : public utils::BaseError
    {
        public:
            NetworkError(const std::string& what, const std::string& where) :
                BaseError(what, where)
            {}
    };

    class Network
    {
        public:
            explicit Network(uint16_t port);
            ~Network();

        protected:
            uint16_t _port;
            boost::asio::io_context _io_context;
    };
} // namespace network
