//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Main class to run client
//

#include "flux/core/flux.hpp"
#include "network/TCPClient.hpp"
#include "network/UDPClient.hpp"
#include "network/datatype.hpp"
#include <Camera2D.hpp>
#include <Window.hpp>
#include <chrono>
#include <memory>
#include <queue>

namespace client {

    constexpr unsigned WINDOW_BASE_WIDTH = 800;
    constexpr unsigned WINDOW_BASE_HEIGHT = 450;
#ifndef _WIN32
    constexpr std::string WINDOW_BASE_NAME = "R-Type";
#else
    const std::string WINDOW_BASE_NAME = "R-Type";
#endif
    constexpr unsigned INPUT_SEND_DELAY = 20;
    constexpr uint8_t NB_REDUNDANCY = 10;

    class Client {
       public:
        Client(const std::string &ip, uint16_t port, const std::string &userPass);

        void run();

       private:
        flux::ECS _ecs;
        flux::runtimeHooks _hooks;

        std::string _ip;
        unsigned _port = 0;
        unsigned _inputIndex = 0;
        std::string _userPass = "";

        std::unique_ptr<raylib::Window> _window = nullptr;
        std::unique_ptr<raylib::Camera2D> _camera = nullptr;
        std::unique_ptr<client::network::TCPClient> _networkTCPClient = nullptr;
        std::unique_ptr<client::network::UDPClient> _networkUDPClient = nullptr;

        std::queue<component::PlayerInput> _inputs;
        ::network::ClientTCPSentInfo _gameInfo;
        std::chrono::steady_clock::time_point _lastInputSend;
        component::PlayerInput _lastSentInput;

        bool _gameLaunch = false;

        bool _chatTyping = false;
        std::string _chatBuffer;
        std::deque<std::pair<std::string, int>> _chatLog;

        void _initHooks();
        void _initNetworkableHooks();
        void _registerBase();
        void _sendPlayerInput(const std::unordered_map<flux::Entity, std::vector<std::any>> &componentStore,
            const std::chrono::steady_clock::time_point &now);

        void _handleChatInput();
        void _renderChat();
    };
}  // namespace client
