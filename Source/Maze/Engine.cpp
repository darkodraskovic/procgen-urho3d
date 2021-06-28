#include <Urho3D/IO/Log.h>

#include "Engine.h"
#include "Crawler.h"

using namespace Maze;

Engine::Engine(Context* context) : Object(context) {}

void Engine::Register() {
    context_->RegisterSubsystem<Maze::Crawler>();
}
void Engine::Start() {
    URHO3D_LOGINFO("Maze Start()");
    
    GetSubsystem<Maze::Crawler>()->Start();
}
