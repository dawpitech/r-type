//
// EPITECH PROJECT, 2025
// r-type
// File description:
// mapLoader
//

#pragma once

#include <functional>
#include <LDtkLoader/Level.hpp>
#include <LDtkLoader/Project.hpp>
#include <LDtkLoader/World.hpp>
#include <optional>
#include "flux/core/flux.hpp"

namespace map
{
    class MapLoader
    {
        public:
            explicit MapLoader(flux::ECS& ecs);

            void initializeGame();

        private:
            void _getMapTiles();
            void _getCollider();

            ldtk::Project _project;
            std::string _ldtkFile;
            flux::ECS& _ecs;
            std::optional<std::reference_wrapper<const ldtk::World>> _world;
            std::optional<std::reference_wrapper<const ldtk::Level>> _level;
    };
} // namespace map
