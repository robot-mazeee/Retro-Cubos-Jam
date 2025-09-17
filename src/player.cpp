#include "player.hpp"

#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/reflection/external/primitives.hpp>

#include <cubos/engine/input/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(Player)
{
    return cubos::core::ecs::TypeBuilder<Player>("Player")
        .withField("speed", &Player::speed)
        .build();
}

void playerPlugin(Cubos& cubos)
{
    cubos.depends(inputPlugin);
    cubos.depends(transformPlugin);

    cubos.component<Player>();

    cubos.system("move player").call([](Input& input, const DeltaTime& dt, Query<Player&, Position&> players) {
        for (auto [player, position] : players)
        {
            glm::vec3 inputVec = {
                input.axis("player-move-lateral"),
                0.0f,
                input.axis("player-move-longitudinal")
            };
            position.vec += dt.value() * player.speed * inputVec;
        }
    });
}
