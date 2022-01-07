#include "CameraController.h"

#include "../Subsystems/ControllerManager.h"

using namespace FPS;

CameraController::CameraController(Context* context) : LogicComponent(context) {
  SetUpdateEventMask(USE_UPDATE);
}

void CameraController::Update(float timeStep) {
  if (GetSubsystem<Urho3D::UI>()->GetFocusElement()) return;

  auto* input = GetSubsystem<Urho3D::Input>();

  if (controls_.IsDown(CTRL_FORWARD))
    node_->Translate(Vector3::FORWARD * moveSpeed_ * timeStep);
  if (controls_.IsDown(CTRL_BACK))
    node_->Translate(Vector3::BACK * moveSpeed_ * timeStep);
  if (controls_.IsDown(CTRL_LEFT))
    node_->Translate(Vector3::LEFT * moveSpeed_ * timeStep);
  if (controls_.IsDown(CTRL_RIGHT))
    node_->Translate(Vector3::RIGHT * moveSpeed_ * timeStep);
  if (controls_.IsDown(CTRL_DOWN))
    node_->Translate(Vector3::DOWN * moveSpeed_ * timeStep);
  if (controls_.IsDown(CTRL_UP))
    node_->Translate(Vector3::UP * moveSpeed_ * timeStep);

  controls_.pitch_ = Clamp(controls_.pitch_, -90.0f, 90.0f);
  node_->SetRotation(Quaternion(controls_.pitch_, controls_.yaw_, 0.0f));
}

void CameraController::Sync() {
  controls_.yaw_ = node_->GetRotation().YawAngle();
  controls_.pitch_ = node_->GetRotation().PitchAngle();
}
