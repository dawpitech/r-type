//
// EPITECH PROJECT, 2025
// r-type
// File description:
// damageSystem
//

#include "components/Collider.hpp"
#include "components/Health.hpp"
#include "components/Mob.hpp"
#include "components/Player.hpp"
#include "flux/core/flux.hpp"

flux::View DamageSystemView(const flux::ECS &ecs)
{
    return ecs.GenerateViewFromComponents<component::Collider, component::Health>();
}

void improovePlayerScore(flux::ECS &ecs)
{
    const auto playerView = ecs.GenerateViewFromComponents<component::Player>();
    const auto allPlayerEntities = ecs.QueryViewNotExclusive(playerView);

    for (auto playerEntity : allPlayerEntities) {
        if (!ecs.HasComponent<component::Player>(playerEntity) ||
            !ecs.HasComponent<component::Health>(playerEntity))
            return;
        auto player = ecs.GetComponent<component::Player>(playerEntity);
        auto playerHealth = ecs.GetComponent<component::Health>(playerEntity);
        if (playerHealth.healthPoint > 0) {
            player.score += 1;
        }
        ecs.AddOrReplace(playerEntity, player);
        std::cout << std::format("Player with score {}", player.score) << std::endl;
    }
}

void DamageSystem(flux::ECS &ecs, const std::vector<flux::Entity> &entities)
{
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity &entity = entities[i];
        auto collider = ecs.GetComponent<component::Collider>(entity);

        if (!collider.hasCollide)
            continue;
        auto health = ecs.GetComponent<component::Health>(entity);
        if (health.healthPoint >= 10)
            health.healthPoint -= 10;
        else {
            health.healthPoint = 0;
        }
        if (ecs.HasComponent<component::Mob>(entity) && health.healthPoint == 0) {
            improovePlayerScore(ecs);
        }
        collider.hasCollide = false;
        ecs.AddOrReplace<component::Collider>(entity, collider);
        ecs.AddOrReplace<component::Health>(entity, health);
    }
}
