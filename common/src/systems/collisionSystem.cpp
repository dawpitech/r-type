//
// EPITECH PROJECT, 2025
// r-type
// File description:
// collisionSystem
//

#include <vector>

#include "components/Collider.hpp"
#include "components/Transform.hpp"
#include "flux/core/flux.hpp"
#include "vector4.hpp"

flux::View CollisionSystemView(const flux::ECS &ecs) {
  return ecs
      .GenerateViewFromComponents<component::Collider, component::Transform>();
}

static bool checkRectCollision(utils::Vector4<float> firstRect,
                               component::Transform firstTransform,
                               utils::Vector4<float> secondRect,
                               component::Transform secondTransform) {
  float left1 = firstTransform.pos.x;
  float right1 = firstTransform.pos.x + (firstRect.w * firstTransform.scale.x);
  float top1 = firstTransform.pos.y;
  float bottom1 = firstTransform.pos.y + (firstRect.h * firstTransform.scale.y);

  float left2 = secondTransform.pos.x;
  float right2 =
      secondTransform.pos.x + (secondRect.w * secondTransform.scale.x);
  float top2 = secondTransform.pos.y;
  float bottom2 =
      secondTransform.pos.y + (secondRect.h * secondTransform.scale.y);

  return (left1 < right2 && right1 > left2 && top1 < bottom2 && bottom1 > top2);
}

static void handleCollision(component::Collider &collider,
                            component::Collider &colliderOther) {
  collider.hasCollide = true;
  colliderOther.hasCollide = true;
}

void CollisionSystem(flux::ECS &ecs,
                     const std::vector<flux::Entity> &entities) {
  for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
    auto collider = ecs.GetComponent<component::Collider>(entities[i]);
    auto transform = ecs.GetComponent<component::Transform>(entities[i]);

    if (!collider.isActive)
      continue;
    for (int y = i - 1; y >= 0; --y) {
      auto colliderOther = ecs.GetComponent<component::Collider>(entities[y]);
      auto transformOther = ecs.GetComponent<component::Transform>(entities[y]);

      if (!colliderOther.isActive)
        continue;

      bool canCollide = (collider.mask & colliderOther.layer) != 0 ||
                        (colliderOther.mask & collider.layer) != 0;
      if (!canCollide)
        continue;
      utils::Vector4<float> rect{};
      rect.x = collider.srcX;
      rect.y = collider.srcY;
      rect.h = collider.srch;
      rect.w = collider.srcw;
      if (checkRectCollision(rect, transform, rect, transformOther)) {
        handleCollision(collider, colliderOther);
        ecs.AddOrReplace<component::Collider>(entities[i], collider);
        ecs.AddOrReplace<component::Collider>(entities[y], colliderOther);
      }
    }
  }
}
