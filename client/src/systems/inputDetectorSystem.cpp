/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** inputDetectorSystem.cpp
*/

#include <components/NetworkIdentification.hpp>
#include <raylib-cpp.hpp>
#include "components/PlayerInput.hpp"

#include "inputDetectorSystem.hpp"

flux::View InputDetectorSystemView(const flux::ECS& ecs)
{
    return ecs.GenerateViewFromComponents<component::PlayerInput, component::NetworkIdentification>();
}

static void handleGamePad(component::PlayerInput& input)
{
    const float leftStickDeadzoneX = 0.1f;
    const float leftStickDeadzoneY = 0.1f;
    const float rightStickDeadzoneX = 0.1f;
    const float rightStickDeadzoneY = 0.1f;
    const float leftTriggerDeadzone = -0.9f;
    const float rightTriggerDeadzone = -0.9f;
    int gamepad = 0;
    if (IsGamepadAvailable(gamepad)) {
        float axisX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X);
        float axisY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y);

        if (axisX < -leftStickDeadzoneX)
            input.move_left = true;
        if (axisX > leftStickDeadzoneX)
            input.move_right = true;
        if (axisY < -leftStickDeadzoneY)
            input.move_up = true;
        if (axisY > leftStickDeadzoneY)
            input.move_down = true;

        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT))
            input.move_left = true;
        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
            input.move_right = true;
        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP))
            input.move_up = true;
        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
            input.move_down = true;

        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) ||
            IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) ||
            IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT) ||
            IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_UP))
            input.shoot = true;

        if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1) ||
            IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1))
            input.shoot = true;

        float rightTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER);
        float leftTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_TRIGGER);

        if (rightTrigger > rightTriggerDeadzone || leftTrigger > leftTriggerDeadzone)
            input.shoot = true;
    }
}

static void handleKeyboard(component::PlayerInput& input) {
        if (raylib::Keyboard::IsKeyDown(KEY_UP))
            input.move_up = true;
        if (raylib::Keyboard::IsKeyDown(KEY_DOWN))
            input.move_down = true;
        if (raylib::Keyboard::IsKeyDown(KEY_LEFT))
            input.move_left = true;
        if (raylib::Keyboard::IsKeyDown(KEY_RIGHT))
            input.move_right = true;
        if (raylib::Keyboard::IsKeyDown(KEY_SPACE))
            input.shoot = true;
        if (raylib::Keyboard::IsKeyDown(KEY_P))
            input.colorblind = true;
        if (raylib::Keyboard::IsKeyDown(KEY_M))
            input.colorblind = false;

        if (raylib::Keyboard::IsKeyUp(KEY_UP))
            input.move_up = false;
        if (raylib::Keyboard::IsKeyUp(KEY_DOWN))
            input.move_down = false;
        if (raylib::Keyboard::IsKeyUp(KEY_LEFT))
            input.move_left = false;
        if (raylib::Keyboard::IsKeyUp(KEY_RIGHT))
            input.move_right = false;
        if (raylib::Keyboard::IsKeyUp(KEY_SPACE))
            input.shoot = false;
}

void InputDetectorSystem(flux::ECS& ecs, const std::vector<flux::Entity>& entities)
{
    for (const flux::Entity entity : entities) {
        component::PlayerInput& input = ecs.GetComponent<component::PlayerInput>(entity);

        input.move_up = false;
        input.move_down = false;
        input.move_left = false;
        input.move_right = false;
        input.shoot = false;

        handleKeyboard(input);
        handleGamePad(input);
    }
}
