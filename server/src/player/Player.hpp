//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Class to handle single player
//

#pragma once

#include <format>

#include <string>

#include "components/PlayerInput.hpp"
#include "network/TCP/TCPInfo.hpp"
#include "network/UDP/UDPNetwork.hpp"

namespace game {

    constexpr unsigned BASE_ENTITY = 999;

    class Player {
       public:
        explicit Player(const network::ConnectionInfo &info, network::UDPNetwork &network,
            network::UDPNetwork &voiceNetwork)
            : _id(info.uuid), _ip(info.ip), _network(network), _voiceNetwork(voiceNetwork)
        {}

        ~Player() = default;

        [[nodiscard]] const std::string &getId() const { return this->_id; }

        void storeInfo(const network::ClientTCPReceivedInfo &info)
        {
            utils::Logger::debug(std::format(
                "Player {} on port {} and voice on port {}", this->_id, info.portUDP, info.portVoiceChat));
            this->_udpPort = info.portUDP;
            this->_voicePort = info.portVoiceChat;
        }

        void storeInput(component::PlayerInput input)
        {
            this->_lastInput = input;
            this->_inputIndex += 1;
        }

        void assignEntity(unsigned entity) { this->_entity = entity; }

        void assignRoom(uint8_t room) { this->_room = room; }

        unsigned getEntity() const { return this->_entity; }

        uint8_t getRoom() const { return this->_room; }

        void sendData(const std::string &string)
        {
            this->_network.sendData(this->_ip, this->_udpPort, string);
        }

        void sendVoice(const std::string &string)
        {
            this->_voiceNetwork.sendData(this->_ip, this->_voicePort, string);
        }

        component::PlayerInput getInput() { return this->_lastInput; };

        unsigned getInputIndex() { return this->_inputIndex; }

        std::string soundBuffer;

       private:
        network::UDPNetwork &_network;
        network::UDPNetwork &_voiceNetwork;
        component::PlayerInput _lastInput;
        std::string _id;
        unsigned _entity = BASE_ENTITY;
        uint16_t _score = 0;
        uint16_t _udpPort = 0;
        uint16_t _voicePort = 0;
        uint8_t _room = 0;
        unsigned _inputIndex = 0;
        std::string _ip;
    };
}  // namespace game
