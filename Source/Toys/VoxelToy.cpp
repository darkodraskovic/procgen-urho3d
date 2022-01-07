#include "VoxelToy.h"

#include "../FPS/Components/CameraController.h"
#include "../FPS/Subsystems/ControllerManager.h"
#include "../FPS/Subsystems/SceneManager.h"
#include "../ProcGen/Subsystems/TextureCreator.h"
#include "../Voxels/Subsystems/Utils.h"
#include "../Voxels/Subsystems/World.h"

using namespace Toy;

VoxelToy::VoxelToy(Context* context) : Object(context) {}

void VoxelToy::Start() {
  SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(VoxelToy, HandleKeyDown));

  CreateVoxels();
  SetupPlayer();
}

void VoxelToy::CreateVoxels() {
  auto* cache = GetSubsystem<ResourceCache>();
  auto* ui = GetSubsystem<UI>();

  auto* sceneManager = GetSubsystem<FPS::SceneManager>();

  XMLFile* xmlFile = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
  ui->GetRoot()->SetDefaultStyle(xmlFile);
  ProgressBar* bar = new ProgressBar(context_);
  bar->SetName("ProgressBar");
  bar->SetRange(100);
  bar->SetValue(0);

  ui->GetRoot()->AddChild(bar);
  bar->SetAlignment(Urho3D::HA_CENTER, Urho3D::VA_BOTTOM);
  bar->SetStyleAuto();
  bar->SetSize(800, 30);

  auto* world = GetSubsystem<Voxels::World>();
  auto* utils = GetSubsystem<Voxels::Utils>();

  world->SetRoot(sceneManager->GetScene());

  // material
  ProcGen::TextureCreator* textureCreator =
      GetSubsystem<ProcGen::TextureCreator>();
  auto* image = cache->GetResource<Image>("Textures/VoxelTerrain.png");
  auto* texture = textureCreator->CreateImageTexture(image);
  texture->SetFilterMode(Urho3D::FILTER_NEAREST);

  auto* techDiff =
      cache->GetResource<Technique>("CoreData/Techniques/Diff.xml");
  // auto* techNoTexture =
  // cache->GetResource<Technique>("CoreData/Techniques/NoTexture.xml"); auto*
  // techNoTextureVCol =
  // cache->GetResource<Technique>("CoreData/Techniques/NoTextureVCol.xml");
  // auto* techDiffLightMap =
  // cache->GetResource<Technique>("CoreData/Techniques/DiffLightMap.xml");
  // auto* techDiffAO =
  // cache->GetResource<Technique>("CoreData/Techniques/DiffAO.xml");
  auto* material = new Material(context_);
  material->SetTechnique(0, techDiff);
  material->SetTexture(Urho3D::TU_DIFFUSE, texture);
  world->SetMaterial(material);

  world->size_ = {8, 4, 8};

  world->CreateColumns();
  world->Build();

  Vector3 size = world->GetWorldSize();
  auto* cam = sceneManager->GetScene()->GetChild("Camera");
  cam->SetPosition(Vector3::UP * size.y_ + Vector3::RIGHT * size.x_);
  cam->LookAt(size / 2);
  cam->Translate(Vector3::BACK * size.y_);

  camController_ = cam->GetComponent<FPS::CameraController>();
  camController_->Sync();
}

void VoxelToy::SetupPlayer() {
  auto* sceneManager = GetSubsystem<FPS::SceneManager>();

  Node* playerNode = sceneManager->CreatePlayer();
  sceneManager->GetScene()->AddChild(playerNode);

  Node* lightNode = playerNode->CreateChild("DirectionalLight");
  lightNode->Translate(Vector3::UP * 2);
  auto* light = lightNode->CreateComponent<Light>();
  light->SetLightType(Urho3D::LIGHT_POINT);
  light->SetBrightness(.5);

  Vector3 size = GetSubsystem<Voxels::World>()->GetWorldSize();
  playerNode->SetPosition(size / 2 + Vector3::UP * size.y_ / 2);

  charController_ = playerNode->GetComponent<FPS::CharacterController>();
  charController_->SetEnabled(firstPerson_);
}

void VoxelToy::HandleKeyDown(StringHash eventType, VariantMap& eventData) {
  using namespace KeyDown;
  int key = eventData[P_KEY].GetInt();

  if (key == KEY_TAB) {
    firstPerson_ = !firstPerson_;
    GetSubsystem<FPS::SceneManager>()->SetFPS(firstPerson_);
  }
}
