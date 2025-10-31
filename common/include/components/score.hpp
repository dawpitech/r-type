//
// EPITECH PROJECT, 2025
// r-type
// File description:
// Score component
//

#pragma once

#include "flux/core/Serialization.hpp"

namespace component
{
    struct Score
    {
            int maxScore;
            bool isLaunched;
            Score(int maxScore = 0): maxScore(maxScore), isLaunched(false) {}
            REFLECT(maxScore, isLaunched)
    };
} // namespace component
