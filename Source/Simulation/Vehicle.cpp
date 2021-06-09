#include <Urho3D/Core/StringUtils.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Math/Quaternion.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/IO/Log.h>

#include "Vehicle.h"

using namespace Simulation;

Vehicle::Vehicle(Context* context) : LogicComponent(context) {
    SetUpdateEventMask(USE_UPDATE);
}

void Vehicle::Start() {
}

// void Vehicle::OnSetEnabled() {
//     URHO3D_LOGINFO("SET enabled VEHICLE");
// }

void Vehicle::DelayedStart() {
    auto* body = node_->GetComponent<RigidBody>();
    body->SetUseGravity(false);
    body->SetAngularFactor({0,1,0});
    body->SetLinearFactor({1,1,0});
}

void Vehicle::Update(float timeStep) {
    if (target_) {
        Steer(target_->GetPosition());
    }
}

void Vehicle::Steer(const Vector3& target) {
    auto* body = node_->GetComponent<RigidBody>();
    
    Vector3 desired = target - node_->GetPosition();
    desired.Normalize();
    desired *= maxSpeed_;
    Vector3 steer = desired - body->GetLinearVelocity();
    steer = steer.ReNormalized(0, maxSpeed_);
    
    body->ApplyForce(steer);

    node_->SetRotation(Lerp(node_->GetRotation(), Quaternion(Atan2(desired.y_, desired.x_)), angularIntertia_));
}
