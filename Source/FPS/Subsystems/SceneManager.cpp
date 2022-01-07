
#include "SceneManager.h"

#include "../Components/CameraController.h"
#include "../Components/CharacterController.h"
#include "../Subsystems/ControllerManager.h"

using namespace FPS;

SceneManager::SceneManager(Context* context) : Object(context) {}

void SceneManager::Start() {
  CreateScene();
  SetupViewport();
  SetupCamController();
}

void SceneManager::CreateScene() {
  scene_ = new Scene(context_);

  scene_->CreateComponent<Octree>();
  scene_->CreateComponent<PhysicsWorld>();
  debugRenderer_ = scene_->CreateComponent<DebugRenderer>();

  scene_->CreateChild("Camera")->CreateComponent<Camera>();
}

void SceneManager::SetupLights() {
  Node* zoneNode = scene_->CreateChild("Zone");
  auto* zone = zoneNode->CreateComponent<Zone>();
  zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
  zone->SetFogColor(Color(0.2f, 0.2f, 0.2f));
  zone->SetFogStart(200.0f);
  zone->SetFogEnd(300.0f);
  float ambientIntensity = .25;
  zone->SetAmbientColor(
      Urho3D::Color(ambientIntensity, ambientIntensity, ambientIntensity));

  Node* lightNode = scene_->CreateChild("DirectionalLight");
  lightNode->SetDirection(Vector3::DOWN + Vector3::RIGHT + Vector3::BACK);
  auto* light = lightNode->CreateComponent<Light>();
  light->SetLightType(LIGHT_DIRECTIONAL);
  // light->SetSpecularIntensity(1.5f);
  light->SetCastShadows(true);
  // light->SetPerVertex(true);
}

void SceneManager::CreateSkybox(const String& material) {
  auto* cache = GetSubsystem<ResourceCache>();

  // Create skybox. The Skybox component is used like StaticModel, but it will
  // be always located at the camera, giving the illusion of the box planes
  // being far away. Use just the ordinary Box model and a suitable material,
  // whose shader will generate the necessary 3D texture coordinates for cube
  // mapping
  Node* skyNode = scene_->CreateChild("Sky");
  skyNode->SetScale(500.0f);  // The scale actually does not matter
  auto* skybox = skyNode->CreateComponent<Skybox>();
  skybox->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
  skybox->SetMaterial(cache->GetResource<Material>(material));
}

void SceneManager::SetupViewport() {
  auto* cam = scene_->GetChild("Camera")->GetComponent<Camera>();
  SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cam));
  GetSubsystem<Renderer>()->SetViewport(0, viewport);
}

void SceneManager::SetupEffects() {
  auto* cache = GetSubsystem<ResourceCache>();
  auto* viewport = GetSubsystem<Renderer>()->GetViewport(0);
  SharedPtr<RenderPath> effectRenderPath = viewport->GetRenderPath()->Clone();
  // effectRenderPath->Append(cache->GetResource<XMLFile>("PostProcess/GreyScale.xml"));
  // effectRenderPath->SetShaderParameter("BloomMix", Vector2(1.1f, .7f));
  // effectRenderPath->SetEnabled("Bloom", true);
  // viewport->SetRenderPath(effectRenderPath);
}

void SceneManager::SetupCamController() {
  auto* camController =
      scene_->GetChild("Camera")->CreateComponent<CameraController>();
  auto* ctrlManager = GetSubsystem<FPS::ControllerManager>();
  ctrlManager->SetControls(&camController->controls_);
}

Scene* SceneManager::GetScene() { return scene_; }

// TODO: move this code to separate module
Node* SceneManager::CreatePlayer() {
  Node* node = scene_->CreateChild("Player");
  node->CreateComponent<FPS::CharacterController>();

  auto* body = node->CreateComponent<RigidBody>();
  body->SetCollisionLayer(1);
  body->SetMass(1.0f);
  body->SetAngularFactor(Vector3::ZERO);
  body->SetCollisionEventMode(COLLISION_ALWAYS);

  auto* shape = node->CreateComponent<CollisionShape>();
  shape->SetCapsule(0.7f, 1.8f, Vector3(0.0f, 0.9f, 0.0f));

  return node;
}

void SceneManager::SetFPS(bool enabled) {
  auto* charController =
      scene_->GetChild("Player")->GetComponent<FPS::CharacterController>();
  auto* camController =
      scene_->GetChild("Camera")->GetComponent<FPS::CameraController>();

  Urho3D::Controls* ctrl =
      enabled ? &charController->controls_ : &camController->controls_;
  GetSubsystem<FPS::ControllerManager>()->SetControls(ctrl);

  camController->Sync();

  camController->SetEnabled(!enabled);
  charController->SetEnabled(enabled);
}
