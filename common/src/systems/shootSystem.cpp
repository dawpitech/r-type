//
// EPITECH PROJECT, 2025
// r-type
// File description:
// shootSystem
//

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

constexpr float SCALE_X = 0.5;
constexpr float SCALE_Y = 0.5;
constexpr int ROTATION = 0;
constexpr float PROJECTILE_SPEED = 10.0f;

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
            render::SpriteData proj = render::SDLManager::load("./assets/playerProjectile.gif");
            render::SpriteData player = render::SDLManager::load("./assets/player.gif");
            ecs.Add<component::Projectile>(projectile, component::Projectile(component::ProjectileType::PLAYER, PROJECTILE_SPEED));
            ecs.Add<component::sprite>(projectile, proj.texture);
            ecs.Add<component::animation>(projectile, component::animation(proj.spriteMap, true));
            ecs.Add<component::Transform>(
                projectile,
                component::Transform(playerTransform.pos.x + (player.frameSize.x * playerTransform.scale.x),
                                     playerTransform.pos.y + (player.frameSize.y * playerTransform.scale.y) / 2, ROTATION, SCALE_X,
                                     SCALE_Y));
            ecs.Add<component::Velocity>(projectile, component::Velocity(0, 0));
            ecs.Add<component::collider>(projectile,
                                         component::collider(component::PLAYER_PROJECTILE, component::MOB,
                                                             0, 0, proj.frameSize.x, proj.frameSize.y));
            ecs.Add<component::Health>(projectile);
            input.shoot = false;
        }
    }
}
