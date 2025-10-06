
#pragma once

#include <string>
#include <memory>
#include "global/utils/error.hpp"
#include "client/network/TCPClient.hpp"

namespace rTypeClient
{
    class GameError : public utils::BaseError
    {
        public:
            explicit GameError(const std::string& msg, const std::string& where) : utils::BaseError(msg, where) {}
    };

    class Game
    {
        public:
            Game() = default;
            ~Game() = default;

            void launchGame(const std::string& serverIp, uint16_t serverPort);

        private:
            bool _running = true;
            std::unique_ptr<client::network::TCPClient> _networkClient;
            
            void _setupNetwork(const std::string& serverIp, uint16_t serverPort);
    };
} // namespace rTypeClient
