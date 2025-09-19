#include "player.hpp"

#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/ecs/name.hpp>
#include <cubos/core/reflection/external/primitives.hpp>

#include <cubos/engine/input/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>
#include <cubos/engine/collisions/plugin.hpp>
#include <cubos/engine/collisions/raycast.hpp>
#include <cubos/engine/collisions/colliding_with.hpp>
#include <cubos/engine/fixed_step/plugin.hpp>

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(Player)
{
    return cubos::core::ecs::TypeBuilder<Player>("Player").withField("speed", &Player::speed).build();
}

void playerPlugin(Cubos& cubos)
{
    cubos.depends(inputPlugin);
    cubos.depends(transformPlugin);
    cubos.depends(collisionsPlugin);
    cubos.depends(fixedStepPlugin);

    cubos.component<Player>();

    cubos.system("move player")
        .tagged(fixedStepTag)
        .call([](Input& input, const FixedDeltaTime& dt, Raycast raycast, Query<Player&, Position&> players,
                 Query<const cubos::core::ecs::Name&> nameQuery) {
            // Physics variables ---------------------------------------------------
            constexpr auto BASE_GRAVITY = glm::vec3{0.0f, -9.83f, 0.0f};
            constexpr auto FALLING_GRAVITY = 2.0f * BASE_GRAVITY;
            // ---------------------------------------------------------------------
            constexpr auto LEVEL_GEOMETRY_MASK = 2;
            // ---------------------------------------------------------------------

            for (auto [player, position] : players)
            {
                // -----
                // Input
                // -----
                auto inputVec = glm::vec3 {
                    input.axis("player-move-lateral"),
                    input.axis("player-move-vertical"),
                    input.axis("player-move-longitudinal")
                };
                position.vec += dt.value * player.speed * inputVec;

                /*
                Gravity during ballistic jumping
                --------------------------------
                velocity += timestep * acceleration;
                position += timestep * (velocity + timestep * acceleration / 2);
                --------------------------------
                (Makes the simulation more correct when dt.value() varies)
                */
                const bool isPlayerFalling = player.velocity.y < 0.0f;
                const auto gravity = isPlayerFalling ? FALLING_GRAVITY : BASE_GRAVITY;

#if 0
                player.velocity += dt.value * gravity;
                position.vec += dt.value * (player.velocity + dt.value * gravity / 2.0f);
#endif

                // -------
                // Raycast
                // -------
                constexpr glm::vec3 DOWN = { 0.0f, -1.0f, 0.0f };
                constexpr float capsuleLength = 8.0f;
                constexpr float capsuleRadius = 2.0f;
                constexpr float halfCapsuleHeight = capsuleLength / 2.0f + capsuleRadius;
                constexpr float nudge = 0.2f;
                const glm::vec3 localBottom = (halfCapsuleHeight - nudge) * DOWN;
                const glm::vec3 playerBottom = position.vec + localBottom;

                if (auto hit = raycast.fire({.origin = playerBottom, .direction = DOWN, .mask = LEVEL_GEOMETRY_MASK}))
                {
                    auto entityNameMatch = nameQuery.at(hit->entity);
                    auto entityName = (entityNameMatch) ? std::get<0>(*entityNameMatch).value : "<???>";
                    CUBOS_DEBUG("Raycast HIT entity {} @ ({}, {}, {})\tdist = {}", entityName, hit->point.x,
                                hit->point.y, hit->point.z, glm::distance(playerBottom, hit->point));
                }

                // -----
                // Debug
                // -----
                CUBOS_DEBUG("velocity = ({}, {}, {})", player.velocity.x, player.velocity.y, player.velocity.z);
                CUBOS_DEBUG("position = ({}, {}, {})", position.vec.x, position.vec.y, position.vec.z);
                CUBOS_DEBUG("pl. bot. = ({}, {}, {})", playerBottom.x, playerBottom.y, playerBottom.z);
            }
        });
}
