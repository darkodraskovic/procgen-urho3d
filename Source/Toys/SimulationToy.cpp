#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Graphics/CustomGeometry.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Resource/ResourceCache.h>

#include "../ProcGen/Subsystems/SceneManager.h"
#include "../ProcGen/Subsystems/ModelCreator.h"
#include "../ProcGen/Subsystems/GeometryCreator.h"

#include "SimulationToy.h"
#include "../Simulation/Vehicle.h"

using namespace Toy;

SimulationToy::SimulationToy(Context* context) : Object(context) {}

void SimulationToy::Start() {
    // URHO3D_LOGINFO("Geom Toy START");
    
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(SimulationToy, HandleUpdate));
    scene_ = GetSubsystem<ProcGen::SceneManager>()->GetScene();

    auto* cache = GetSubsystem<ResourceCache>();
    scene_->GetComponent<Skybox>(true)->SetEnabled(false);

    auto* camNode = scene_->GetChild("Camera");
    camNode->Translate(Vector3::BACK * 32);

    // CreateTestGeometry();
    auto* vehicleNode = CreateVehicle();

    auto* modelCreator = GetSubsystem<ProcGen::ModelCreator>();
    auto* targetNode = modelCreator->CreateStockModel("Box");
    scene_->AddChild(targetNode);
    targetNode->SetName("Target");
    targetNode->SetScale(.5);

    float dist = 2;
    targetNode->SetPosition({dist,dist,0});

    vehicleNode->GetComponent<Simulation::Vehicle>()->target_ = targetNode;
}

Node* SimulationToy::CreateVehicle() {
    // Geometry
    auto* geomCreator = GetSubsystem<ProcGen::GeometryCreator>();
    CustomGeometry *customGeom = geomCreator->CreateCustomGeometry(PrimitiveType::TRIANGLE_LIST, 3);
    scene_->AddChild(customGeom->GetNode());

    Vector3 normal{0,0,-1};
    float y = 1.0 / Sqrt(3.0) * .75;

    customGeom->DefineVertex(Vector3(-.5, y, 0));
    customGeom->DefineNormal(normal);
    customGeom->DefineColor(Color::BLUE);
    customGeom->DefineVertex(Vector3(.5, 0, 0));
    customGeom->DefineNormal(normal);
    customGeom->DefineColor(Color::GREEN);
    customGeom->DefineVertex(Vector3(-.5, -y, 0));
    customGeom->DefineNormal(normal);
    customGeom->DefineColor(Color::BLUE);

    customGeom->Commit();

    // RigidBody
    auto* node = customGeom->GetNode();
    auto* body = node->CreateComponent<RigidBody>();
    body->SetMass(1);
    // body->SetUseGravity(false);
    // body->SetAngularFactor({0,1,0});
    // body->SetLinearFactor({1,1,0});
    
    auto* shape = node->CreateComponent<CollisionShape>();
    shape->SetCustomConvexHull(customGeom);

    // Vehicle
    node->SetName("Vehicle");
    auto* vehicle = node->CreateComponent<Simulation::Vehicle>();
    return vehicle->GetNode();
}

void SimulationToy::HandleUpdate(StringHash eventType, VariantMap& eventData) {
    // auto* targetNode = scene_->GetChild("Target");

    // Vector3 targetPos{
    //     4 * Cos(M_RADTODEG * GetSubsystem<Time>()->GetElapsedTime()),
    //     4 * Sin(M_RADTODEG * GetSubsystem<Time>()->GetElapsedTime()),
    //     0};
    // targetNode->SetPosition(targetPos);

    auto* camNode = scene_->GetChild("Camera");
    auto* vehicleNode = scene_->GetChild("Vehicle");
    camNode->SetPosition(vehicleNode->GetPosition());
    camNode->Translate(Vector3::BACK * 32);
}
