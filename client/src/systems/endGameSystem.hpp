/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EndGameSystem.cpp
*/

#include "flux/core/flux.hpp"

#include <CameraRaylib.hpp>
#include <Functions.hpp>
#include <components/Transform.hpp>

flux::View endGameSystemView(const flux::ECS& ecs);
void endGameCameraSystem(flux::ECS &ecs, const std::vector<flux::Entity> &entities);
