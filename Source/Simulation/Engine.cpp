#include "Engine.h"

using namespace Simulation;

Simulation::Engine::Engine(Context* context) : Object(context) {
  context_->RegisterFactory<Simulation::Vehicle>();
}

void Simulation::Engine::Start() {
  URHO3D_LOGINFO("Simulation Start()");
  // context_->RegisterFactory<Simulation::Vehicle>();
}
