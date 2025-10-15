/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** network Identification
*/

#pragma once

#include "network/datatype.hpp"
#include <cstring>

namespace component {
    struct NetworkIdentification {
        char uuid[network::BUFFERSIZE];

        NetworkIdentification()
        {
            std::memset(this->uuid, '\0', network::BUFFERSIZE);
        }
        REFLECT(uuid)
    };

}  // namespace component
