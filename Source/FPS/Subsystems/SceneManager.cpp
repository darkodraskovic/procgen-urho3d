#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/RenderPath.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Resource/XMLFile.h>

#include "SceneManager.h"
#include "../Components/CameraController.h"

using namespace FPS;

SceneManager::SceneManager(Context* context) : Object(context) {}

void SceneManager::Start() {
    CreateScene();
    CreateSkybox("Materials/Space.xml");
    SetupViewport();
}

void SceneManager::CreateScene() {
    auto* cache = GetSubsystem<ResourceCache>();

    scene_ = new Scene(context_);

    scene_->CreateComponent<Octree>();
    scene_->CreateComponent<PhysicsWorld>();
    debugRenderer_ = scene_->CreateComponent<DebugRenderer>();

    Node* zoneNode = scene_->CreateChild("Zone");
    auto* zone = zoneNode->CreateComponent<Zone>();
    zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
    zone->SetFogColor(Color(0.2f, 0.2f, 0.2f));
    zone->SetFogStart(200.0f);
    zone->SetFogEnd(300.0f);
    float ambientIntensity = .25;
    zone->SetAmbientColor(Urho3D::Color(ambientIntensity, ambientIntensity, ambientIntensity));

    Node* lightNode = scene_->CreateChild("DirectionalLight");
    lightNode->SetDirection(Vector3::DOWN + Vector3:: RIGHT + Vector3::BACK);
    auto* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
    // light->SetSpecularIntensity(1.5f);
    light->SetCastShadows(true);
    // light->SetPerVertex(true);
}

void SceneManager::CreateSkybox(const String& material) {
    auto* cache = GetSubsystem<ResourceCache>();
    
    // Create skybox. The Skybox component is used like StaticModel, but it will be always located at the camera, giving the
    // illusion of the box planes being far away. Use just the ordinary Box model and a suitable material, whose shader will
    // generate the necessary 3D texture coordinates for cube mapping
    Node* skyNode = scene_->CreateChild("Sky");
    skyNode->SetScale(500.0f); // The scale actually does not matter
    auto* skybox = skyNode->CreateComponent<Skybox>();
    skybox->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
    skybox->SetMaterial(cache->GetResource<Material>(material));
}

void SceneManager::SetupViewport() {
    auto* camNode_ = scene_->CreateChild("Camera");
    camNode_->CreateComponent<CameraController>();
    auto* camera = camNode_->CreateComponent<Camera>();

    auto* renderer = GetSubsystem<Renderer>();
    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, camNode_->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);

    auto* cache = GetSubsystem<ResourceCache>();
    SharedPtr<RenderPath> effectRenderPath = viewport->GetRenderPath()->Clone();
    // effectRenderPath->Append(cache->GetResource<XMLFile>("PostProcess/GreyScale.xml"));
    // effectRenderPath->SetShaderParameter("BloomMix", Vector2(1.1f, .7f));
    // effectRenderPath->SetEnabled("Bloom", true);
    // viewport->SetRenderPath(effectRenderPath);
}

Scene *SceneManager::GetScene() { return scene_; }
