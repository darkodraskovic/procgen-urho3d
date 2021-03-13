#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Resource/ResourceCache.h>

#include "SceneManager.h"
#include "../Components/CameraController.h"

using namespace ProcGen;

SceneManager::SceneManager(Context* context) : Object(context) {}

void SceneManager::CreateScene() {
    auto* cache = GetSubsystem<ResourceCache>();

    scene_ = new Scene(context_);

    scene_->CreateComponent<Octree>();

    Node* zoneNode = scene_->CreateChild("Zone");
    auto* zone = zoneNode->CreateComponent<Zone>();
    zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
    zone->SetFogColor(Color(0.2f, 0.2f, 0.2f));
    zone->SetFogStart(200.0f);
    zone->SetFogEnd(300.0f);

    Node* lightNode = scene_->CreateChild("DirectionalLight");
    lightNode->SetDirection(Vector3(0, -1.0f, 0.8f));
    auto* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
    light->SetColor(Color(0.4f, 1.0f, 0.4f));
    light->SetSpecularIntensity(1.5f);

    debugRenderer_ =  scene_->CreateComponent<DebugRenderer>();
}

void SceneManager::SetupViewport() {
    cameraNode_ = scene_->CreateChild("camera");
    cameraNode_->SetPosition(Vector3(0.0f, 4.0f, -8.0f));
    cameraNode_->LookAt(Vector3::ZERO);
    cameraNode_->CreateComponent<ProcGen::CameraController>();
    auto* camera = cameraNode_->CreateComponent<Camera>();
    
    auto* renderer = GetSubsystem<Renderer>();
    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);    
}

Scene* SceneManager::GetScene() {
    return scene_;
}
