/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Music
*/

#pragma once

#include <raylib-cpp.hpp>
#include <string>

namespace component
{
    struct MusicCmp
    {
            std::string musicPath;
            Music music;

            MusicCmp(std::string musicPath) : musicPath(musicPath) { this->music = LoadMusicStream(musicPath.c_str()); }
    };

} // namespace component
