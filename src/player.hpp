#pragma once

#include <cubos/engine/prelude.hpp>

#include <glm/vec3.hpp>

struct Player
{
    CUBOS_REFLECT;

    // Movement feel
    glm::vec3 velocity = { 0.0f, 0.0f, 0.0f };

    // Reflected fields
    float speed{1.0F};     // Speed of the player
};

void playerPlugin(cubos::engine::Cubos& cubos);
