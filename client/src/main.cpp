/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** main.cpp
*/

#include <iostream>
#include <ostream>
#include "flux/core/flux.hpp"
#include "global/components/health.hpp"
#include "global/systems/healthSystem.hpp"

int main()
{
    flux::ECS ecs;

    const flux::Entity e1 = ecs.newEntity();

    ecs.Add<Health>(e1);
    std::cout << static_cast<int>(ecs.GetComponent<Health>(e1).healthPoint) << std::endl;
    HealthSystem(ecs, e1);
    std::cout << static_cast<int>(ecs.GetComponent<Health>(e1).healthPoint) << std::endl;
    return 0;
}
