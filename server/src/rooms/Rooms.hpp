//
// EPITECH PROJECT, 2025
// R-Type
// File description:
// Class for rooms logic
//

#pragma once

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <ctime>
#include <deque>
#include <mutex>
#include <ostream>

#include "flux/core/Serialization.hpp"
#include "flux/core/flux.hpp"
#include "player/Player.hpp"

namespace Room {
    constexpr uint8_t BASEROOMPLAYER = 4;
    constexpr uint8_t NB_SNAPSHOTS = 10;

    class Room {
       public:
        explicit Room(std::size_t roomNumber, std::uint8_t nbPlayers = BASEROOMPLAYER);
        ~Room();

        void run();
        void stop();
        void clear(std::uint8_t nbPlayers);

        bool addPlayer(game::Player &player);

        bool isRoomFull();
        void notifyRoomFull();
        void waitRoomReady();

       private:
        std::mutex _roomMutex;
        std::mutex _readyMutex;
        std::condition_variable _readyCondition;
        std::condition_variable _fullCondition;
        flux::ECS _ecs;
        std::vector<std::reference_wrapper<game::Player>> _players;
        std::deque<std::unordered_map<flux::Entity, std::vector<std::any>>> _snapshots;
        std::chrono::steady_clock::time_point _networkClock;
        std::uint8_t _nbPlayerMax;
        std::size_t _roomNumber;
        bool _isReady = false;
        std::atomic<bool> _isRunning = true;

        void _setRoomReady();
        void _waitRoomFull();
        void _assignPlayerToEntity(game::Player &player);
        void _initHooks(flux::runtimeHooks &hooks);
        void _initUpdateHook(flux::runtimeHooks &hooks);
        void _initNetworkHook(flux::runtimeHooks &hooks);

        void _serializeComponent(const unsigned entity, const std::any &component, std::ostringstream &out);
        void _getSnapshot(std::unordered_map<flux::Entity, std::vector<std::any>> &store);
        void _sendSnapshotToPlayer(std::ostringstream &serializedData);

        template <typename T>
        void _getSerializedComponent(
            flux::Entity entity, const std::any &component, std::ostream &serializedData)
        {
            if (!(component.type() == typeid(T)))
                return;
            const auto &comp = std::any_cast<const T &>(component);
            serializedData << flux::SerializerHandler<T>::serialize(this->_ecs, entity, comp) << std::endl;
        }
    };
}  // namespace Room
