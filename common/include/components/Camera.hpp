//
// EPITECH PROJECT, 2025
// r-type
// File description:
// Camera component
//

#pragma once

#include "flux/core/Serialization.hpp"

namespace component
{
    struct Camera
    {
            float posX;
            float posY;
            float offsetX;
            float offsetY;
            int rotation;
            int zoom;

            Camera(float posX = 0, float posY = 0, float offsetX = 0, float offsetY = 0, int rotation = 0, int zoom = 1) :
                posX(posX), posY(posY), offsetX(offsetX), offsetY(offsetY), rotation(rotation), zoom(zoom)
            {}
            REFLECT(posX, posY, offsetX, offsetY, rotation, zoom)
    };
} // namespace component
