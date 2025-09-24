//
// EPITECH PROJECT, 2025
// r-type
// File description:
// healthSystem
//

#include "flux/core/flux.hpp"
#include "global/components/health.hpp"

void HealthSystem(flux::ECS &ecs, flux::Entity e1)
{
    if (!ecs.HasComponent<Health>(e1))
        return;
    auto& [healthPoint] = ecs.GetComponent<Health>(e1);
    if (healthPoint >= 10) {
        healthPoint -= 10;
    } else {
        healthPoint = 0;
    }
}
