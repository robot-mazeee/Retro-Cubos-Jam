#include "freakingAudio.hpp"
#include "player.hpp"

#include <cubos/core/ecs/reflection.hpp>

#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/audio/plugin.hpp>
#include <cubos/engine/prelude.hpp>
#include <cubos/engine/transform/position.hpp>
#include <cubos/engine/transform/rotation.hpp>
#include <cubos/engine/transform/child_of.hpp>
#include <cubos/engine/physics/components/velocity.hpp>

using namespace cubos::engine;

static const Asset<Audio> BackgroundMusic = AnyAsset("/assets/audio/cubos_jam_demo.mp3");
static const Asset<Audio> WingFlapSFX = AnyAsset("/assets/audio/malfunctioning_bird_wings.mp3");

void freakingAudioPlugin(Cubos& cubos)
{
    cubos.depends(audioPlugin);
    cubos.depends(assetsPlugin);
    cubos.depends(playerPlugin);

    cubos.startupSystem("create an audio listener").after(audioStateInitTag).call([](Commands cmds) {
        cmds.create()
            .add(Position{{8.0F, 1.0F, -2.0F}})
            .add(Velocity{.vec = {1.0F, 1.0F, 1.0F}})
            .add(Rotation{})
            .add(AudioListener{true});
    });

    cubos.startupSystem("create a background audio stream").after(audioStateInitTag).after(assetsTag).call([](Commands cmds, Assets& assets) {
        cmds.create()
            .add(Position{{8.0F, 1.0F, -2.0F}})
            .add(Velocity{.vec = {1.0F, 1.0F, 1.0F}})
            .add(Rotation{})
            .add(AudioSource{ .looping = true, .gain = 0.75, .sound = BackgroundMusic })
            .add(AudioPlay{});
    });
    
    cubos.startupSystem("create the wing flap audio source").after(audioStateInitTag).after(assetsTag).call([](Commands cmds, Assets& assets, Query<Entity, Player&> players) {
        for (auto [playerEntity, player] : players)
        {
            player.wings = cmds.create()
                .named("wing flap SFX")
                .relatedTo(playerEntity, ChildOf{})
                .add(AudioSource{ .looping = false, .gain = 1.00, .sound = WingFlapSFX })
                .entity();
        }
    });

    //cubos.observer("play sounds immediately")
    //    .onAdd<AudioSource>()
    //    .without<AudioPlay>()
    //    .call([](Commands cmds, Query<Entity> query) {
    //        for( auto [ ent ] : query) {
    //            cmds.add(ent, AudioPlay{});
    //        }
    //    });
}
