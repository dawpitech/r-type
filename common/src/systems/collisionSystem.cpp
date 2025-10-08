//
// EPITECH PROJECT, 2025
// r-type
// File description:
// collisionSystem
//

#include <cstddef>
#include <iostream>
#include <vector>
#include "components/Collider.hpp"
#include "components/Transform.hpp"
#include "flux/core/flux.hpp"
#include "spriteHandler.hpp"

flux::View CollisionSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::collider, component::Transform>();
}

static bool checkRectCollision(sprite::Rect firstRect, component::Transform firstTransform, sprite::Rect secondRect,
                               component::Transform secondTransform)
{
    float left1 = firstTransform.pos.x;
    float right1 = firstTransform.pos.x + firstRect.destX;
    float top1 = firstTransform.pos.y;
    float bottom1 = firstTransform.pos.y + firstRect.destY;

    float left2 = secondTransform.pos.x;
    float right2 = secondTransform.pos.x + secondRect.destX;
    float top2 = secondTransform.pos.y;
    float bottom2 = secondTransform.pos.y + secondRect.destY;

    return (left1 < right2 && right1 > left2 && top1 < bottom2 && bottom1 > top2);
}

static void handleCollision(component::collider& collider, component::collider& colliderOther)
{
    collider.hasCollide = true;
    colliderOther.hasCollide = true;
}

void CollisionSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i) {
        auto& collider = ecs.GetComponent<component::collider>(entities[i]);
        auto& transform = ecs.GetComponent<component::Transform>(entities[i]);

        if (!collider.isActive)
            continue;
        for (int y = i - 1; y >= 0; --y) {
            auto& colliderOther = ecs.GetComponent<component::collider>(entities[y]);
            auto& transformOther = ecs.GetComponent<component::Transform>(entities[y]);

            if (!colliderOther.isActive)
                continue;

            bool canCollide = (collider.mask & colliderOther.layer) != 0 || (colliderOther.mask & collider.layer) != 0;
            if (!canCollide)
                continue;
            if (checkRectCollision(collider.rect, transform, colliderOther.rect, transformOther))
                handleCollision(collider, colliderOther);
        }
    }
}
