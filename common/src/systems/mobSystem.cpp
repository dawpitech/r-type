//
// EPITECH PROJECT, 2025
// r-type
// File description:
// mob System
//

#include "components/Mob.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"

flux::View MobSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::Mob, component::Transform, component::Velocity>();
}

void MobSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    return;
    /*
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        const auto& tranform = ecs.GetComponent<component::Transform>(entity);
        auto& velocity = ecs.GetComponent<component::Velocity>(entity);
        auto& mob = ecs.GetComponent<component::mob>(entity);

        velocity.x = (-mob.speed);
        if (tranform.pos.x > 0 && tranform.pos.x < static_cast<float>(render::SDLManager::getWindowWidth()))
            mob.isShooting = true;
    }
    */
}
