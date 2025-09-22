/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** r-type.cpp
*/

#include <iostream>

#include "flux/core/flux.hpp"

struct Transform
{
    float x = 0;
    float y = 0;
};

struct Velocity
{
    float x = 0;
    float y = 0;
};

int main() {
    flux::ECS ecs;

    const flux::Entity e1 = ecs.newEntity();
    std::cout << e1 << std::endl;
    std::cout << ecs.HasComponent<Transform>(e1) << std::endl;
    ecs.Add<Transform>(e1, Transform{ 10, 10 });
    std::cout << ecs.HasComponent<Transform>(e1) << std::endl;
}
