//
// EPITECH PROJECT, 2025
// r-type
// File description:
// MobStartPosition component
//

#pragma once

#include "flux/core/Serialization.hpp"

namespace component
{
    struct MobStartPosition
    {
        int startY;
        int amplitude;
        int speed;
        int currentPosModificator;
        bool movingUp;

        MobStartPosition(float startY = 0, int amplitude = 10, int speed = 1)
            : startY(startY), amplitude(amplitude), speed(speed),
              currentPosModificator(0), movingUp(true) {}

        REFLECT(startY, amplitude, speed, currentPosModificator, movingUp)
    };
} // namespace component
