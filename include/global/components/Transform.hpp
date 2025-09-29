/*
** EPITECH PROJECT, 2025
** flux
** File description:
** Transform.hpp
*/

#pragma once

#include <cstdint>
#include "global/vector2.hpp"

namespace component
{
    struct Transform
    {
            utils::Vector2<float> pos;
            int8_t rotation;
            utils::Vector2<float> scale;

            Transform(const float x, const float y, const char rotation,
                      const float scaleX, const float scaleY) :
                pos(x, y), rotation(rotation), scale(scaleX, scaleY)
            {}
    };
} // namespace component
