/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** network Identification
*/

#pragma once

#include "network/datatype.hpp"

namespace component
{
    struct NetworkIdentification
    {
            char uuid[network::BUFFERSIZE];

            REFLECT(uuid)
    };

} // namespace component
