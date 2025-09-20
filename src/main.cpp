#include <cubos/core/ecs/name.hpp>

#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/defaults/plugin.hpp>
#include <cubos/engine/input/plugin.hpp>
#include <cubos/engine/render/lights/environment.hpp>
#include <cubos/engine/render/voxels/palette.hpp>
#include <cubos/engine/scene/plugin.hpp>
#include <cubos/engine/settings/plugin.hpp>
#include <cubos/engine/tools/plugin.hpp>
#include <cubos/engine/utils/free_camera/plugin.hpp>
#include <cubos/engine/voxels/plugin.hpp>

#include <cubos/engine/collisions/colliding_with.hpp>
#include <cubos/engine/collisions/collision_layers.hpp>
#include <cubos/engine/collisions/collision_mask.hpp>
#include <cubos/engine/collisions/plugin.hpp>
#include <cubos/engine/collisions/shapes/box.hpp>
#include <cubos/engine/physics/plugin.hpp>
#include <cubos/engine/physics/plugins/gravity.hpp>
#include <cubos/engine/physics/solver/plugin.hpp>

#include <cubos/engine/audio/plugin.hpp>
#include <cubos/engine/audio/play.hpp>
#include <cubos/engine/audio/audio.hpp>
#include <cubos/engine/audio/listener.hpp>
#include <cubos/engine/audio/source.hpp>

#include "obstacle.hpp"
#include "player.hpp"
#include "spawner.hpp"
#include "question-mark-block.hpp"

using namespace cubos::engine;

static const Asset<Scene> SceneAsset = AnyAsset("/assets/scenes/main.cubos");
static const Asset<VoxelPalette> PaletteAsset = AnyAsset("/assets/main.pal");
static const Asset<InputBindings> InputBindingsAsset = AnyAsset("/assets/input.bind");

int main(int argc, char** argv)
{
    Cubos cubos{argc, argv};
    
    // -------
    // Plugins
    // -------

    cubos.plugin(defaultsPlugin);
    cubos.plugin(freeCameraPlugin);
    cubos.plugin(toolsPlugin);

    // cubos.plugin(physicsSolverPlugin);
    cubos.plugin(gravityPlugin);

    cubos.plugin(playerPlugin);
    
    // To spawn my arbitrary question mark block:
    // cubos.plugin(questionMarkBlockPlugin);

    cubos.plugin(audioPlugin);


    // -------
    // Systems
    // -------

    cubos.startupSystem("configure settings").before(settingsTag).call([](Settings& settings) {
        settings.setString("assets.app.osPath", APP_ASSETS_PATH);
        settings.setString("assets.builtin.osPath", BUILTIN_ASSETS_PATH);
    });

    cubos.startupSystem("set the palette, environment, input bindings and spawn the scene")
        .tagged(assetsTag)
        .call([](Commands commands, const Assets& assets, RenderPalette& palette, Input& input,
                 RenderEnvironment& environment) {
            palette.asset = PaletteAsset;
            environment.ambient = {0.1F, 0.1F, 0.1F};
            environment.skyGradient[0] = {0.2F, 0.4F, 0.8F};
            environment.skyGradient[1] = {0.6F, 0.6F, 0.8F};
            input.bind(*assets.read(InputBindingsAsset));
            commands.spawn(assets.read(SceneAsset)->blueprint()).named("main");
        });

    cubos.system("restart the game on input")
        .call([](Commands cmds, const Assets& assets, const Input& input, Query<Entity> all) {
            if (input.justPressed("restart"))
            {
                for (auto [ent] : all)
                {
                    cmds.destroy(ent);
                }

                cmds.spawn(assets.read(SceneAsset)->blueprint()).named("main");
            }
        });

    cubos.system("detect player vs obstacle collisions")
        .call([](Query<const Player&, const CollidingWith&, Entity, const cubos::core::ecs::Name&> collisions) {
            for (auto [player, collidingWith, entity, name] : collisions)
            {
                CUBOS_INFO("Player collided with <{}>!", name.value);
                (void)player; // here to shut up 'unused variable warning', you can remove it
            }
        });

    cubos.run();
}
