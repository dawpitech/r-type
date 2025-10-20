/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Animation.hpp
*/

#pragma once

#include "flux/core/Serialization.hpp"

namespace component
{
    struct Animation
    {
        int nbFrames;
        int currentFrame;
        bool loop;
        int width;
        int height;
        std::chrono::milliseconds msBtwFrames;
        std::string animationTexturePath;
        std::chrono::time_point<std::chrono::steady_clock> lastFrameTimepoint;

        Animation(const int nbFrames, const int msBtwFrames, const std::string& animationTexturePath, const bool loop, const int width, const int height)
            : nbFrames(nbFrames), currentFrame(0), loop(loop), width(width), height(height), msBtwFrames(msBtwFrames), animationTexturePath(animationTexturePath), lastFrameTimepoint(std::chrono::steady_clock::now()) {}
    };

} // namespace component
