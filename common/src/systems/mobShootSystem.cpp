//
// EPITECH PROJECT, 2025
// r-type
// File description:
// mob shoot system
//

#include "components/Collider.hpp"
#include "components/Health.hpp"
#include "components/Mob.hpp"
#include "components/Projectile.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"

constexpr float SCALE_X = 0.5;
constexpr float SCALE_Y = 0.5;
constexpr int ROTATION = 0;
constexpr float PROJECTILE_SPEED = 5.0f;

flux::View MobShootSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::Mob, component::Transform>();
}

void MobShootSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    using clock = std::chrono::steady_clock;
    static auto prev = clock::now();

    auto now = clock::now();
    std::chrono::duration<double> frameTime = now - prev;
    prev = now;

    double deltaTime = frameTime.count();

    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity& entity = entities[i];
        auto& mob = ecs.GetComponent<component::Mob>(entity);

        if (!mob.canShoot)
            continue;

        mob.shootCooldown -= static_cast<float>(deltaTime);

        if (mob.shootCooldown <= 0.0f && mob.isShooting) {
            const auto &mobTransform = ecs.GetComponent<component::Transform>(entity);
            const flux::Entity projectile = ecs.newEntity();

            ecs.Add<component::Transform>(
                projectile, component::Transform(mobTransform.pos.x,
                                                 mobTransform.pos.y, 0, 1, 1));
            ecs.Add<component::Velocity>(projectile);
            ecs.Add<component::Projectile>(projectile, component::Projectile(1));
            ecs.Add<component::Collider>(
                projectile,
                component::Collider(component::CollisionLayer::MOB_PROJECTILE, component::CollisionLayer::WALL | component::CollisionLayer::PLAYER,
                                    mobTransform.pos.x, mobTransform.pos.y, 32,
                                    32));

            mob.shootCooldown = mob.shootRate;
            mob.isShooting = false;
        }
    }
}
