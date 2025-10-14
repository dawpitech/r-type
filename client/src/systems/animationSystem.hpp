/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** animationSystem.hpp
*/

#pragma once

#include <flux/core/flux.hpp>

flux::View AnimationSystemView(const flux::ECS& ecs);
void AnimationSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities);
