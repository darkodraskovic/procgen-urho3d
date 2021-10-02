
#include <Urho3D/IO/Log.h>

#include "Engine.h"

using namespace Voxels;

Engine::Engine(Context* context) : Object(context) {
    context_->RegisterSubsystem<Voxels::Utils>();
    context_->RegisterSubsystem<Voxels::World>();

    context_->RegisterFactory<Voxels::Block>();
    context_->RegisterFactory<Voxels::Chunk>();
}

void Engine::Start() {
    URHO3D_LOGINFO("Voxels Start()");

    GetSubsystem<Voxels::World>()->Start();
}
