#include "question-mark-block.hpp"

#include <glm/vec3.hpp>

#include <cubos/engine/render/voxels/grid.hpp> // RenderVoxelGrid
#include <cubos/engine/render/mesh/plugin.hpp> // renderMeshPlugin, renderMeshPoolInitTag
#include <cubos/engine/transform/plugin.hpp> // Position

using namespace cubos::engine;

// static const Asset<VoxelGrid> QuestionMarkBlockAsset = AnyAsset("/assets/voxels/blocks/bad-question-mark-block.grd");
static const Asset<VoxelGrid> QuestionMarkBlockAsset = AnyAsset("dcf85c3c-ff04-4e50-924c-c86ecfd204e6");

void questionMarkBlockPlugin(Cubos& cubos)
{
    cubos.depends(renderMeshPlugin); // defines the tag renderMeshPoolInitTag

    cubos
        .startupSystem("Load question mark block")
        .after(renderMeshPoolInitTag) // to avoid No such resource "State#render_mesh_plugin"
        // .tagged(assetsTag)         // This also works, and is shown in https://docs.cubosengine.org/examples-engine-voxels.html
        .call([](Commands commands) {
            commands.create()
                .named("The Cube")
                .add(RenderVoxelGrid{QuestionMarkBlockAsset})
                .add(Position{glm::vec3{16.0, 1.0, 8.0}});
        });
}
