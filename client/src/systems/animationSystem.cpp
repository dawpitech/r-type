/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** animationSystem.cpp
*/

#include "animationSystem.hpp"
#include "components/Animation.hpp"

#include <TextureStore.hpp>
#include <components/Transform.hpp>

flux::View AnimationSystemView(const flux::ECS& ecs) {
    return ecs.GenerateViewFromComponents<component::Transform, component::Animation>();
}

void AnimationSystem(flux::ECS &ecs, const std::vector<flux::Entity> &entities) {
    for (const flux::Entity entity : entities) {
        const auto& transform = ecs.GetComponent<component::Transform>(entity);
        auto& animation = ecs.GetComponent<component::Animation>(entity);

        const auto timeSinceLastFrame = std::chrono::steady_clock::now() - animation.lastFrameTimepoint;
        const auto timesinceLastFrameInMs = std::chrono::duration_cast<std::chrono::milliseconds>(timeSinceLastFrame);

        if (timesinceLastFrameInMs > animation.msBtwFrames) {
            animation.lastFrameTimepoint = std::chrono::steady_clock::now();
            animation.currentFrame++;

            if (animation.currentFrame >= animation.nbFrames) {
                if (animation.loop) {
                    animation.currentFrame = 0;
                } else {
                    ecs.Remove<component::Animation>(entity);
                    continue;
                }
            }
        }

        TextureStore::getInstance()
            .getTexture(
                animation.animationTexturePath,
                animation.currentFrame * animation.width,
                0,
                animation.width,
                animation.height)
            .Draw(
                static_cast<int>(transform.pos.x),
                static_cast<int>(transform.pos.y));
    }
}
