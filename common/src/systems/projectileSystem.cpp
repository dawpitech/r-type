//
// EPITECH PROJECT, 2025
// r-type
// File description:
// projectile System
//

// #include "components/Animation.hpp"
#include "components/Collider.hpp"
#include "components/Health.hpp"
#include "components/Projectile.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"

flux::View ProjectileSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::Projectile, component::Velocity, component::Transform>();
}

void ProjectileSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        auto velocity = ecs.GetComponent<component::Velocity>(entity);
        auto transform = ecs.GetComponent<component::Transform>(entity);
        auto proj = ecs.GetComponent<component::Projectile>(entity);
        if (!ecs.HasComponents<component::Sprite>(entity)) {
            ecs.Add<component::Sprite>(entity,
                                       component::Sprite("assets/player_shoot_spritesheet.png", 96, 0, 32, 32, 1));
        }

        if (proj.type == 0) {
            velocity.x = proj.speed;
            if (transform.pos.x > 3000) {
                //ecs.Remove<component::Transform>(entity);
                //ecs.Remove<component::Velocity>(entity);
                //ecs.Remove<component::Projectile>(entity);
                //ecs.Remove<component::Collider>(entity);
                //ecs.Remove<component::Health>(entity);
               ecs.DeleteEntity(entity);
            }
        }
        if (proj.type == 1) {
            velocity.x = (-proj.speed);
            if (transform.pos.x <= 0) {
                ecs.DeleteEntity(entity);
                continue;
            }
        }
        ecs.AddOrReplace<component::Velocity>(entity, velocity);
    }
}
