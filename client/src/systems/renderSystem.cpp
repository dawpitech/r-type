//
// EPITECH PROJECT, 2025
// r-type
// File description:
// healthSystem
//

#include "TextureStore.hpp"

#include <raylib-cpp.hpp>

#include <vector>

#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "flux/core/flux.hpp"

#include <components/Animation.hpp>

flux::View RenderSystemView(const flux::ECS &ecs) {
  return ecs
      .GenerateViewFromComponents<component::Sprite, component::Transform>();
}

void RenderSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    std::vector<std::pair<int, flux::Entity>> sorted_entities;
    sorted_entities.reserve(entities.size());

    for (const flux::Entity entity : entities) {
        const auto& s = ecs.GetComponent<component::Sprite>(entity);
        sorted_entities.emplace_back(s.zHeight, entity);
    }

    std::ranges::stable_sort(sorted_entities,
        [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

    for (const auto& [z, entity] : sorted_entities) {
        if (ecs.HasComponent<component::Animation>(entity))
            continue;
        const auto& sprite = ecs.GetComponent<component::Sprite>(entity);
        const auto& transform = ecs.GetComponent<component::Transform>(entity);

    const auto &texture = TextureStore::getInstance().getTexture(
        sprite.assetPath, sprite.srcX, sprite.srcY, sprite.srcW, sprite.srcH);
    texture.Draw(static_cast<int>(transform.pos.x),
                 static_cast<int>(transform.pos.y));
  }
}
