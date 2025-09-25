//
// EPITECH PROJECT, 2025
// r-type
// File description:
// movement
//

namespace component
{
    struct PlayerInput
    {
            bool move_right = false;
            bool move_left = false;
            bool move_up = false;
            bool move_down = false;
            bool shoot = false;

            float move_speed = 200.0f;
    };
} // namespace component
