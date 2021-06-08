#include <Urho3D/Graphics/Graphics.h>

#include <Urho3D/Input/InputConstants.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/UI/UI.h>

#include "CameraController.h"

using namespace ProcGen;

CameraController::CameraController(Context *context) : LogicComponent(context) {
    SetUpdateEventMask(USE_UPDATE);
}

void CameraController::Update(float timeStep) {
    if (GetSubsystem<Urho3D::UI>()->GetFocusElement())
        return;

    auto* input = GetSubsystem<Urho3D::Input>();

    // Movement speed as world units per second    
    const float MOVE_SPEED = 8.0f;
    // const float MOVE_SPEED = 32.0f;

    using namespace Urho3D;
    
    if (controls_.IsDown(CTRL_FORWARD))
        node_->Translate(Vector3::FORWARD * MOVE_SPEED * timeStep);
    if (controls_.IsDown(CTRL_BACK))
        node_->Translate(Vector3::BACK * MOVE_SPEED * timeStep);
    if (controls_.IsDown(CTRL_LEFT))
        node_->Translate(Vector3::LEFT * MOVE_SPEED * timeStep);
    if (controls_.IsDown(CTRL_RIGHT))
        node_->Translate(Vector3::RIGHT * MOVE_SPEED * timeStep);
    if (controls_.IsDown(CTRL_DOWN))
        node_->Translate(Vector3::DOWN * MOVE_SPEED * timeStep);
    if (controls_.IsDown(CTRL_UP))
        node_->Translate(Vector3::UP * MOVE_SPEED * timeStep);

    controls_.pitch_ = Clamp(controls_.pitch_, -80.0f, 80.0f);
    node_->SetRotation(Quaternion(controls_.pitch_, controls_.yaw_, 0.0f));
}

void CameraController::UpdateRotation() {
    controls_.yaw_ = node_->GetRotation().YawAngle();
    controls_.pitch_ = node_->GetRotation().PitchAngle();
}
