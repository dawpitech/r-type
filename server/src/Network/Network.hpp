//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Class for network connection
//

#pragma once

#include <boost/asio.hpp>

namespace network
{
    class Network
    {
        public:
            Network();
            ~Network();

        protected:
            boost::asio::io_context io_context;
    };
} // namespace network
