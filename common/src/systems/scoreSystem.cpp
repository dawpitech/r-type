//
// EPITECH PROJECT, 2025
// r-type
// File description:
// score System
//

#include "components/Health.hpp"
#include "components/Mob.hpp"
#include "components/Player.hpp"
#include "flux/core/flux.hpp"

flux::View ScoreSystemView(const flux::ECS &ecs)
{
    return ecs.GenerateViewFromComponents<component::Health>();
}

static void improovePlayerScore(flux::ECS &ecs)
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
    }
}

void ScoreSystem(flux::ECS &ecs, const std::vector<flux::Entity> &entities)
{
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        const flux::Entity &entity = entities[i];
        auto health = ecs.GetComponent<component::Health>(entity);
        if (ecs.HasComponent<component::Mob>(entity) && health.healthPoint == 0) {
            improovePlayerScore(ecs);
        }
    }
}
