//
// EPITECH PROJECT, 2025
// r-type
// File description:
// healthSystem
//

#include "flux/core/flux.hpp"
#include "global/components/health.hpp"

void HealthSystem(flux::ECS &ecs, flux::Entity entity)
{
    if (!ecs.HasComponent<component::Health>(entity))
        return;
    auto& [healthPoint] = ecs.GetComponent<component::Health>(entity);
    // TODO: This code is temporary and will be updated in the future.
    // It serves as an example for adapting the code architecture.
    if (healthPoint >= 10) {
        healthPoint -= 10;
    } else {
        healthPoint = 0;
    }
}
