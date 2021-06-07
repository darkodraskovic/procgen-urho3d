#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputConstants.h>

#include "ProcGen/Components/CameraController.h"
#include "Controller.h"

using namespace ProcGen;

Controller::Controller(Context* context) : Object(context) {}

void Controller::Start() {
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Controller, HandleUpdate));
}

void Controller::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    auto* input = GetSubsystem<Input>();
    // Controls& controls = player_ && firstPerson_ ? player_->controls_ :
    //     scene_->GetChild("Camera")->GetComponent<ProcGen::CameraController>()->controls_;

    using namespace ProcGen;
    controls_->Set(CTRL_FORWARD, input->GetKeyDown(KEY_W));
    controls_->Set(CTRL_BACK, input->GetKeyDown(KEY_S));
    controls_->Set(CTRL_LEFT, input->GetKeyDown(KEY_A));
    controls_->Set(CTRL_RIGHT, input->GetKeyDown(KEY_D));

    controls_->Set(CTRL_DOWN, input->GetKeyDown(KEY_Q));
    controls_->Set(CTRL_UP, input->GetKeyDown(KEY_E));

    controls_->Set(CTRL_JUMP, input->GetKeyDown(KEY_SPACE));

    float YAW_SENSITIVITY = 0.1f;
    controls_->yaw_ += (float)input->GetMouseMoveX() * YAW_SENSITIVITY;
    controls_->pitch_ += (float)input->GetMouseMoveY() * YAW_SENSITIVITY;
}

void Controller::SetControls(Urho3D::Controls* controls) {
    controls_ = controls;
}
