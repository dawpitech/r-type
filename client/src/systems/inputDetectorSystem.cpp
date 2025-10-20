/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** inputDetectorSystem.cpp
*/

#include "inputDetectorSystem.hpp"

#include <raylib-cpp.hpp>
#include <components/NetworkIdentification.hpp>

flux::View InputDetectorSystemView(const flux::ECS& ecs)
{
  return ecs.GenerateViewFromComponents<component::PlayerInput, component::NetworkIdentification>();
}

void InputDetectorSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    for (const flux::Entity entity : entities) {
        auto &[move_right, move_left, move_up, move_down, shoot] = ecs.GetComponent<component::PlayerInput>(entity);

        if (raylib::Keyboard::IsKeyDown(KEY_UP))
            move_up = true;
        if (raylib::Keyboard::IsKeyDown(KEY_DOWN))
            move_down = true;
        if (raylib::Keyboard::IsKeyDown(KEY_LEFT))
            move_left = true;
        if (raylib::Keyboard::IsKeyDown(KEY_RIGHT))
            move_right = true;
        if (raylib::Keyboard::IsKeyDown(KEY_SPACE))
            shoot = true;

        if (raylib::Keyboard::IsKeyUp(KEY_UP))
            move_up = false;
        if (raylib::Keyboard::IsKeyUp(KEY_DOWN))
            move_down = false;
        if (raylib::Keyboard::IsKeyUp(KEY_LEFT))
            move_left = false;
        if (raylib::Keyboard::IsKeyUp(KEY_RIGHT))
            move_right = false;
        if (raylib::Keyboard::IsKeyUp(KEY_SPACE))
            shoot = false;
    }
}
