/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** inputDetectorSystem.cpp
*/


#include <raylib-cpp.hpp>
#include <components/NetworkIdentification.hpp>

#include "inputDetectorSystem.hpp"

flux::View InputDetectorSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::PlayerInput, component::NetworkIdentification>();
}

void InputDetectorSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    const float leftStickDeadzoneX = 0.1f;
    const float leftStickDeadzoneY = 0.1f;
    const float rightStickDeadzoneX = 0.1f;
    const float rightStickDeadzoneY = 0.1f;
    const float leftTriggerDeadzone = -0.9f;
    const float rightTriggerDeadzone = -0.9f;
    for (const flux::Entity entity : entities) {
        auto& [move_right, move_left, move_up, move_down, shoot] = ecs.GetComponent<component::PlayerInput>(entity);

        move_up = false;
        move_down = false;
        move_left = false;
        move_right = false;
        shoot = false;

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
        int gamepad = 0;
        if (IsGamepadAvailable(gamepad)) {
            float axisX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X);
            float axisY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y);

            if (axisX < -leftStickDeadzoneX)
                move_left = true;
            if (axisX > leftStickDeadzoneX)
                move_right = true;
            if (axisY < -leftStickDeadzoneY)
                move_up = true;
            if (axisY > leftStickDeadzoneY)
                move_down = true;

            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT))
                move_left = true;
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
                move_right = true;
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP))
                move_up = true;
            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
                move_down = true;

            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) ||
                IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) ||
                IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT) ||
                IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_UP))
                shoot = true;

            if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1) ||
                IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1))
                shoot = true;

            float rightTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER);
            float leftTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_TRIGGER);

            if (rightTrigger > rightTriggerDeadzone || leftTrigger > leftTriggerDeadzone)
                shoot = true;
        }
    }
}
