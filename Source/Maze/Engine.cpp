#include "Engine.h"

using namespace Maze;

Maze::Engine::Engine(Context* context) : Object(context) {
  context_->RegisterSubsystem<Maze::Crawler>();
}

void Maze::Engine::Start() {
  URHO3D_LOGINFO("Maze Start()");

  GetSubsystem<Maze::Crawler>()->Start();
}
