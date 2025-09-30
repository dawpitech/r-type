//
// EPITECH PROJECT, 2025
// R-type
// File description:
// Base function for the network class
//

#include "Network.hpp"
#include "utils/observer.hpp"

network::Network::Network(const uint16_t port) : _port(port) {}

network::Network::~Network() {}

void network::Network::attach(utils::IObserver<network::ReceivedData>& observer)
{
    this->_observers.push_back(observer);
}

void network::Network::notify(const ReceivedData& data)
{
    for (auto& observer : this->_observers) {
        observer.get().update(data);
    }
}
