//
// EPITECH PROJECT, 2025
// r-type
// File description:
// movementSystem
//

#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "flux/core/flux.hpp"
#include <vector>

constexpr auto DAMPING_FORCE = 0.98f;

flux::View MovementSystemView(const flux::ECS &ecs) {
  return ecs
      .GenerateViewFromComponents<component::Transform, component::Velocity>();
}

void MovementSystem(flux::ECS &ecs, const std::vector<flux::Entity> &entities) {
  for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
    const flux::Entity &entity = entities[i];
    auto transform = ecs.GetComponent<component::Transform>(entity);
    auto velocity = ecs.GetComponent<component::Velocity>(entity);

    transform.pos.x += velocity.x;
    transform.pos.y += velocity.y;

    velocity.y *= DAMPING_FORCE;
    velocity.x *= DAMPING_FORCE;
    ecs.AddOrReplace<component::Velocity>(entity, velocity);
    ecs.AddOrReplace<component::Transform>(entity, transform);
  }
}
