//
// EPITECH PROJECT, 2025
// r-type
// File description:
// healthSystem
//

#include <iostream>
#include "components/Health.hpp"
#include "flux/core/flux.hpp"

flux::View HealthSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::Health>();
}

void HealthSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        auto& health = ecs.GetComponent<component::Health>(entity);
        if (health.healthPoint <= 0) {
            continue;
        }
    }
}
