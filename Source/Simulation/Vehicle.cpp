#include <Urho3D/Core/StringUtils.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Math/Quaternion.h>
#include <Urho3D/Math/Random.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Graphics/DebugRenderer.h>

#include "../ProcGen/Subsystems/SceneManager.h"
#include "Vehicle.h"

using namespace Simulation;

Vehicle::Vehicle(Context* context) : LogicComponent(context) {
    SetUpdateEventMask(USE_UPDATE);
}

void Vehicle::Start() {
    // Add default target node
    target_ = new Node(context_);
    target_->SetPosition(node_->GetPosition());
    // SetRandomSeed(0);
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
    if (arrived_) Wander();

    // Scene* scene = GetSubsystem<ProcGen::SceneManager>()->GetScene();
    // auto* debug = scene->GetComponent<DebugRenderer>();
    // debug->AddCircle(target_->GetPosition(), Vector3::FORWARD, wanderRadius_, Color::RED);
}

void Vehicle::Steer(const Vector3& target) {
    auto* body = node_->GetComponent<RigidBody>();
    
    Vector3 desired = target - node_->GetPosition();
    float len = desired.Length();
    if (len < slowdownTreshlod_) {
        desired = desired.Normalized() * ((len / slowdownTreshlod_) * slowdownTreshlod_);
        arrived_ = true;
    } else {
        arrived_ = false;
        desired = desired.Normalized() * maxSpeed_;
    }
    Vector3 steer = desired - body->GetLinearVelocity();
    steer = steer.ReNormalized(0, maxForce_);
    
    body->ApplyForce(steer);

    // Normalized linear interpolation with shortestPath = true
    // node_->SetRotation(node_->GetRotation().Nlerp(Quaternion(Atan2(desired.y_, desired.x_)), angularIntertia_, true));
    // Spherical interpolation
    node_->SetRotation(node_->GetRotation().Slerp(Quaternion(Atan2(desired.y_, desired.x_)), angularIntertia_));
}

void Vehicle::Wander() {
    Vector3 orientation = node_->GetComponent<RigidBody>()->GetLinearVelocity();
    Vector3 center = node_->GetPosition() + orientation.Normalized() * wanderDistance_;
    float x = Cos(Random(-M_PI, M_PI) * M_RADTODEG) * wanderRadius_ + center.x_;
    float y = Sin(Random(-M_PI, M_PI) * M_RADTODEG) * wanderRadius_ + center.y_;
    target_->SetPosition({x, y, 0});
}
