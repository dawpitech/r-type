//
// EPITECH PROJECT, 2025
// r-type
// File description:
// projectile System
//

//#include "components/Animation.hpp"
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
    return;
    /*
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        auto& velocity = ecs.GetComponent<component::Velocity>(entity);
        auto& transform = ecs.GetComponent<component::Transform>(entity);
        const auto& proj = ecs.GetComponent<component::Projectile>(entity);
        #ifdef IS_CLIENT
        if (!ecs.HasComponents<component::sprite, component::animation>(entity)) {
            render::SpriteData proj = render::SDLManager::load("./assets/playerProjectile.gif");
            ecs.Add<component::sprite>(entity, component::sprite(proj.texture));
            ecs.Add<component::animation>(entity, component::animation(proj.spriteMap, true));
        }
        #endif

        if (proj.type == component::ProjectileType::PLAYER) {
            velocity.x = proj.speed;
            if (transform.pos.x > 3000)
                ecs.DeleteEntity(entity);
        }
        if (proj.type == component::ProjectileType::MOB) {
            velocity.x = (-proj.speed);
            if (transform.pos.x <= 0) {
                ecs.DeleteEntity(entity);
            }
        }
    }
    */
}
