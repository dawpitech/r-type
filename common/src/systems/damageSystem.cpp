//
// EPITECH PROJECT, 2025
// r-type
// File description:
// damageSystem
//

#include <cstddef>
#include <iostream>

#include "components/Collider.hpp"
#include "components/Health.hpp"
#include "flux/core/flux.hpp"

flux::View DamageSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::collider, component::Health>();
}

void DamageSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        auto& collider = ecs.GetComponent<component::collider>(entity);

        if (!collider.hasCollide)
            continue;
        auto& health = ecs.GetComponent<component::Health>(entity);
        if (health.healthPoint >= 10)
            health.healthPoint -= 10;
        else {
            health.healthPoint = 0;
        }
        collider.hasCollide = false;
    }
}
