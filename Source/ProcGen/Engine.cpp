#include "Engine.h"

using namespace ProcGen;

ProcGen::Engine::Engine(Context* context) : Object(context) {
  context_->RegisterSubsystem<ProcGen::ModelCreator>();
  context_->RegisterSubsystem<ProcGen::TextureCreator>();
  context_->RegisterSubsystem<ProcGen::MaterialCreator>();
  context_->RegisterSubsystem<ProcGen::GeometryCreator>();

  context_->RegisterFactory<ProcGen::ProcModel>();
}

void ProcGen::Engine::Start() {
  URHO3D_LOGINFO("ProcGen Start()");

  GetSubsystem<ProcGen::ModelCreator>()->Start();
  GetSubsystem<ProcGen::TextureCreator>()->Start();
}
