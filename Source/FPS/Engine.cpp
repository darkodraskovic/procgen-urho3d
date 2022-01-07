#include "Engine.h"

FPS::Engine::Engine(Context* context) : Object(context) {
  context_->RegisterSubsystem<SceneManager>();
  context_->RegisterSubsystem<ControllerManager>();

  context_->RegisterFactory<CameraController>();
  CharacterController::RegisterObject(context_);
}

void FPS::Engine::Start() {
  URHO3D_LOGINFO("ProcGen Start()");

  GetSubsystem<ControllerManager>()->Start();
  GetSubsystem<SceneManager>()->Start();
}
