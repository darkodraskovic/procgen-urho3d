#include "ProcModelToy.h"

#include "../FPS/Components/CameraController.h"
#include "../FPS/Subsystems/SceneManager.h"
#include "../ProcGen/Components/ProcModel.h"

using namespace Toy;

ProcModelToy::ProcModelToy(Context* context) : Object(context) {}

void ProcModelToy::Start() { CreateModel(); }

Node* ProcModelToy::CreateModel() {
  auto* scene = GetSubsystem<FPS::SceneManager>()->GetScene();

  Node* node = scene->CreateChild("ProceduralObject");
  ProcGen::ProcModel* procModel = node->CreateComponent<ProcGen::ProcModel>();

  Vector<Vector3> points = {
      Vector3::LEFT + Vector3::DOWN + Vector3::BACK,
      Vector3::LEFT + Vector3::UP + Vector3::BACK,
      Vector3::RIGHT + Vector3::UP + Vector3::BACK,
      Vector3::RIGHT + Vector3::DOWN + Vector3::BACK,

      Vector3::LEFT + Vector3::DOWN + Vector3::FORWARD,
      Vector3::RIGHT + Vector3::DOWN + Vector3::FORWARD,
      Vector3::RIGHT + Vector3::UP + Vector3::FORWARD,
      Vector3::LEFT + Vector3::UP + Vector3::FORWARD,
  };
  for (auto& p : points) p *= .5;  // [-.5, .5]

  procModel->positions_ = {
      points[0], points[1], points[2], points[3],  // back
      points[0], points[4], points[7], points[1],  // left
      points[4], points[5], points[6], points[7],  // front
      points[3], points[2], points[6], points[5],  // right
      points[0], points[3], points[5], points[4],  // bottom
      points[1], points[7], points[6], points[2],  // up
  };

  procModel->normals_ = {
      Vector3::BACK,    Vector3::BACK,    Vector3::BACK,    Vector3::BACK,
      Vector3::LEFT,    Vector3::LEFT,    Vector3::LEFT,    Vector3::LEFT,
      Vector3::FORWARD, Vector3::FORWARD, Vector3::FORWARD, Vector3::FORWARD,
      Vector3::RIGHT,   Vector3::RIGHT,   Vector3::RIGHT,   Vector3::RIGHT,
      Vector3::DOWN,    Vector3::DOWN,    Vector3::DOWN,    Vector3::DOWN,
      Vector3::UP,      Vector3::UP,      Vector3::UP,      Vector3::UP,
  };

  procModel->colors_ = {
      Color::RED, Color::GREEN, Color::BLUE, Color::MAGENTA,
      Color::RED, Color::GREEN, Color::BLUE, Color::MAGENTA,
      Color::RED, Color::GREEN, Color::BLUE, Color::MAGENTA,
      Color::RED, Color::GREEN, Color::BLUE, Color::MAGENTA,
      Color::RED, Color::GREEN, Color::BLUE, Color::MAGENTA,
      Color::RED, Color::GREEN, Color::BLUE, Color::MAGENTA,
  };

  procModel->uvs_ = {
      Vector2::ZERO, Vector2::UP, Vector2::ONE, Vector2::RIGHT,
      Vector2::ZERO, Vector2::UP, Vector2::ONE, Vector2::RIGHT,
      Vector2::ZERO, Vector2::UP, Vector2::ONE, Vector2::RIGHT,
      Vector2::ZERO, Vector2::UP, Vector2::ONE, Vector2::RIGHT,
      Vector2::ZERO, Vector2::UP, Vector2::ONE, Vector2::RIGHT,
      Vector2::ZERO, Vector2::UP, Vector2::ONE, Vector2::RIGHT,
  };

  procModel->indices_ = {
      0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,  8,  9,  10, 8,  10, 11,
      12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23,
  };

  auto* cache = GetSubsystem<ResourceCache>();

  // technique
  auto* techNoTexture =
      cache->GetResource<Technique>("Techniques/NoTexture.xml");
  auto* techNoTextureUnlit =
      cache->GetResource<Technique>("Techniques/NoTextureUnlit.xml");
  auto* techNoTextureUnlitAlpha =
      cache->GetResource<Technique>("Techniques/NoTextureUnlitAlpha.xml");
  auto* techNoTextureVCol =
      cache->GetResource<Technique>("CoreData/Techniques/NoTextureVCol.xml");

  // material
  auto* mat1 = new Material(context_);
  // mat1->SetTechnique(0, techNoTextureVCol);
  mat1->SetTechnique(0, techNoTextureVCol);

  procModel->material_ = mat1;
  procModel->GenerateData();
  procModel->Commit();
  procModel->SetDrawNormals(true);

  auto* body = node->CreateComponent<RigidBody>();
  body->SetMass(1);
  body->SetUseGravity(false);
  body->SetAngularVelocity(Vector3::ONE);

  auto* camNode = scene->GetChild("Camera");
  camNode->Rotate(Quaternion(30, 180, 0));
  camNode->Translate(Vector3::BACK * 5);
  camNode->GetComponent<FPS::CameraController>()->Sync();

  auto* node2 = scene->CreateChild("ProceduralObject2");
  auto* staticModel = node2->CreateComponent<StaticModel>();
  staticModel->SetModel(node->GetComponent<StaticModel>()->GetModel());
  staticModel->SetMaterial(node->GetComponent<StaticModel>()->GetMaterial());
  scene->AddChild(node2);
  node2->Translate(Vector3::LEFT * 2);
  // object->SetModel(model);
  // node->Translate(Vector3::LEFT);

  return node;
}
