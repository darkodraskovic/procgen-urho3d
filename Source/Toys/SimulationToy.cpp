#include "SimulationToy.h"

#include "../FPS/Subsystems/SceneManager.h"
#include "../ProcGen/Subsystems/GeometryCreator.h"
#include "../ProcGen/Subsystems/ModelCreator.h"
#include "../Simulation/Vehicle.h"

using namespace Toy;

SimulationToy::SimulationToy(Context* context) : Object(context) {}

void SimulationToy::Start() {
  SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(SimulationToy, HandleUpdate));

  scene_ = GetSubsystem<FPS::SceneManager>()->GetScene();
  scene_->GetComponent<Skybox>(true)->SetEnabled(false);

  auto* camNode = scene_->GetChild("Camera");
  camNode->Translate(Vector3::BACK * 32);

  auto* modelCreator = GetSubsystem<ProcGen::ModelCreator>();
  auto* targetNode = modelCreator->CreateStockModel("Box");
  scene_->AddChild(targetNode);
  targetNode->SetName("Target");
  targetNode->SetScale(.5);
  targetNode->SetPosition({2, 2, 0});

  auto* vehicleNode = CreateVehicle();
  vehicleNode->GetComponent<Simulation::Vehicle>()->target_ = targetNode;
}

Node* SimulationToy::CreateVehicle() {
  // Geometry
  auto* geomCreator = GetSubsystem<ProcGen::GeometryCreator>();
  CustomGeometry* customGeom =
      geomCreator->CreateCustomGeometry(PrimitiveType::TRIANGLE_LIST, 3);
  scene_->AddChild(customGeom->GetNode());

  Vector3 normal{0, 0, -1};
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
