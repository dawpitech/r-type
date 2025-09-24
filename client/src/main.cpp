/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** main.cpp
*/

#include <iostream>
#include <ostream>
#include "flux/core/flux.hpp"
#include "global/components/Transform.hpp"
#include "global/components/health.hpp"
#include "global/systems/healthSystem.hpp"

int main()
{
    flux::ECS ecs;

    const flux::Entity e1 = ecs.newEntity();
    const flux::Entity e2 = ecs.newEntity();

    ecs.Add<Transform>(e2, Transform{1, 3});
    ecs.Add<Health>(e1, Health());
    std::cout << static_cast<int>(ecs.GetComponent<Health>(e1).healthPoint) << std::endl;
    HealthSystem(ecs, e1);
    std::cout << static_cast<int>(ecs.GetComponent<Health>(e1).healthPoint) << std::endl;
    return 0;
}
