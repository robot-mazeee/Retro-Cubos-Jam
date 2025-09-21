#pragma once

#include <cubos/engine/prelude.hpp>

#include <glm/vec3.hpp>

struct Player
{
    CUBOS_REFLECT;

    // Reflected fields
    float speed{1.0F};     // Velocity of player
    float jump{1.0F};      // Strength of jump impulse
};

void playerPlugin(cubos::engine::Cubos& cubos);
