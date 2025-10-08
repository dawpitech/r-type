//
// EPITECH PROJECT, 2025
// r-type
// File description:
// projectile System
//

#include "components/Projectile.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"

flux::View ProjectileSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::Projectile, component::Velocity, component::Transform>();
}

void ProjectileSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities) {
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        auto& velocity = ecs.GetComponent<component::Velocity>(entity);

        velocity.x = 1;
    }
}
