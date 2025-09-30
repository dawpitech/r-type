//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Class for network connection
//

#pragma once

#include <boost/asio.hpp>
#include <functional>
#include "utils/error.hpp"
#include "utils/observer.hpp"

namespace network
{
    struct ReceivedData
    {
            virtual ~ReceivedData() = default;
    };

    class NetworkError final : public utils::BaseError
    {
        public:
            NetworkError(const std::string& what, const std::string& where) : BaseError(what, where) {}
    };

    class Network : public utils::ISubject<ReceivedData>
    {
        public:
            explicit Network(uint16_t port);
            ~Network() override;

            void attach(utils::IObserver<ReceivedData>& observer) override;
            void notify(const ReceivedData& data) override;

        protected:
            std::vector<std::reference_wrapper<utils::IObserver<ReceivedData>>> _observers;
            uint16_t _port;
            boost::asio::io_context _io_context;
    };
} // namespace network
