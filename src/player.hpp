#pragma once

#include <cubos/engine/prelude.hpp>
#include <cubos/engine/audio/source.hpp>

#include <glm/vec3.hpp>

struct Player
{
    CUBOS_REFLECT;

    // Reflected fields
    float speed{1.0F};     // Velocity of player
    float jump{1.0F};      // Strength of jump impulse

    cubos::engine::Entity wings;
};

void playerPlugin(cubos::engine::Cubos& cubos);
