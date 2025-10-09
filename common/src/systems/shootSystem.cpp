//
// EPITECH PROJECT, 2025
// r-type
// File description:
// shootSystem
//

#include <iostream>
#include "components/Animation.hpp"
#include "components/Collider.hpp"
#include "components/Health.hpp"
#include "components/Player.hpp"
#include "components/PlayerInput.hpp"
#include "components/Projectile.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"
#include "sdlManager.hpp"

flux::View ShootSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::Player, component::PlayerInput, component::Transform>();
}

void ShootSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        auto& input = ecs.GetComponent<component::PlayerInput>(entity);
        if (input.shoot) {
            auto& playerTransform = ecs.GetComponent<component::Transform>(entity);
            flux::Entity projectile = ecs.newEntity();

            ecs.Add<component::Projectile>(projectile);
            ecs.Add<component::sprite>(
                projectile,
                component::sprite(render::SDLManager::load("./assets/playerProjectile-mod-resize.gif").texture));

            ecs.Add<component::animation>(
                projectile,
                component::animation(render::SDLManager::load("./assets/playerProjectile-mod-resize.gif").spriteMap,
                                     true));

            ecs.Add<component::Transform>(
                projectile,
                component::Transform(
                    playerTransform.pos.x + render::SDLManager::load("./assets/player.gif").frameSize.x + 10,
                    playerTransform.pos.y + render::SDLManager::load("./assets/player.gif").frameSize.y / 2, 0, 1, 1));
            ecs.Add<component::Velocity>(projectile, component::Velocity(1, 0));
            ecs.Add<component::collider>(
                projectile,
                component::collider(
                    component::PLAYER_PROJECTILE, component::MOB,
                    {0, 0, render::SDLManager::load("./assets/playerProjectile-mod-resize.gif").frameSize.x,
                     render::SDLManager::load("./assets/playerProjectile-mod-resize.gif").frameSize.y}));
            ecs.Add<component::Health>(projectile);

            input.shoot = false;
        }
    }
}
