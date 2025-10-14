/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** inputDetectorSystem.hpp
*/

#pragma once

#include <flux/core/flux.hpp>

flux::View InputDetectorSystemView(const flux::ECS& ecs);
void InputDetectorSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities);
