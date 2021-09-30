#include <Urho3D/IO/Log.h>

#include "Engine.h"

using namespace ProcGen;

Engine::Engine(Context* context) : Object(context) {
    context_->RegisterSubsystem<ProcGen::SceneManager>();
    context_->RegisterSubsystem<ProcGen::ModelCreator>();
    context_->RegisterSubsystem<ProcGen::TextureCreator>();
    context_->RegisterSubsystem<ProcGen::MaterialCreator>();
    context_->RegisterSubsystem<ProcGen::GeometryCreator>();
    context_->RegisterSubsystem<ProcGen::Controller>();
    
    context_->RegisterFactory<ProcGen::CameraController>();
    context_->RegisterFactory<ProcGen::ProcModel>();
}


void Engine::Start() {
    URHO3D_LOGINFO("ProcGen Start()");

    GetSubsystem<ProcGen::ModelCreator>()->Start();
    GetSubsystem<ProcGen::TextureCreator>()->Start();
    GetSubsystem<ProcGen::Controller>()->Start();
    GetSubsystem<ProcGen::SceneManager>()->Start();
}
