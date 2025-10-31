//
// EPITECH PROJECT, 2025
// r-type
// File description:
// Score System
//

#include <vector>
#include "components/Mob.hpp"
#include "components/Player.hpp"
#include "components/score.hpp"
#include "flux/core/flux.hpp"

flux::View ScoreSystemView(const flux::ECS& ecs) { return ecs.GenerateViewFromComponents<component::Score>(); }

void ScoreSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    for (int i = 0; i < static_cast<int>(entities.size()); ++i) {
        const flux::Entity& entity = entities[i];
        if (ecs.HasComponent<component::Player>(entity)) {
            auto player = ecs.GetComponent<component::Player>(entity);
            auto score = ecs.GetComponent<component::Score>(entity);

            player.score = 0;

            for (int y = 0; y < static_cast<int>(entities.size()); ++y) {
                if (ecs.HasComponent<component::Mob>(entities[y])) {
                    if (!score.isLaunched)
                        score.maxScore += 1;
                    player.score += 1;
                }
            }
            score.isLaunched = true;

            player.score = score.maxScore - player.score;
            std::cout << "Entity number " << entity << " has " << player.score << " score\n";
            ecs.AddOrReplace(entity, score);
            ecs.AddOrReplace(entity, player);
        }
    }
}
