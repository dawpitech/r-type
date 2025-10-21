//
// EPITECH PROJECT, 2025
// r-type
// File description:
// shootSystem
//

#include "components/Health.hpp"
#include "components/Player.hpp"
#include "components/PlayerInput.hpp"
#include "components/Projectile.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"
#include <iostream>

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
    using clock = std::chrono::steady_clock;
    static auto prev = clock::now();

    auto now = clock::now();
    std::chrono::duration<double> frameTime = now - prev;
    prev = now;

    double deltaTime = frameTime.count();

    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        auto input = ecs.GetComponent<component::PlayerInput>(entity);
        auto playerCpn = ecs.GetComponent<component::Player>(entity);
        
        playerCpn.shootCooldown -= static_cast<float>(deltaTime);
        if (playerCpn.shootCooldown <= 0.0f && input.shoot) {
            std::cout << "Shoot Shoot\n";
            auto& playerTransform = ecs.GetComponent<component::Transform>(entity);
            flux::Entity projectile = ecs.newEntity();
        ecs.Add<component::Transform>(
            projectile, component::Transform(playerTransform.pos.x, playerTransform.pos.y, 0, 1, 1));
        ecs.Add<component::Velocity>(projectile);
        ecs.Add<component::Projectile>(projectile, component::Projectile(0));
        // ecs.Add<component::Animation>(projectileEntity,
        //     component::Animation(
        //         4, 200, "assets/player_shoot_spritesheet.png", true, 32, 32));

            ecs.Add<component::Health>(projectile);
            playerCpn.shootCooldown = 0.2;
        }
        ecs.AddOrReplace<component::PlayerInput>(entity, input);
        ecs.AddOrReplace<component::Player>(entity, playerCpn);
    }
}
