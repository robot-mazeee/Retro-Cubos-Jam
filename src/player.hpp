#pragma once

#include <cubos/engine/prelude.hpp>

struct Player
{
    CUBOS_REFLECT;

    float speed{1.0F};     // Speed of the player
};

void playerPlugin(cubos::engine::Cubos& cubos);
