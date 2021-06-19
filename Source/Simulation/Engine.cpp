#include <Urho3D/IO/Log.h>

#include "Engine.h"
#include "Vehicle.h"

using namespace Simulation;

Engine::Engine(Context* context) : Object(context) {}

void Engine::Register() {
    context_->RegisterFactory<Simulation::Vehicle>();
}
void Engine::Start() {
    URHO3D_LOGINFO("Simulation Start()");
    // context_->RegisterFactory<Simulation::Vehicle>();
}
