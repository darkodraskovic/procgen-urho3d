#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/RenderPath.h>
#include <Urho3D/Resource/XMLFile.h>

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
    light->SetSpecularIntensity(1.5f);
    light->SetPerVertex(true);

    debugRenderer_ =  scene_->CreateComponent<DebugRenderer>();
}

void SceneManager::SetupViewport() {
    auto* camNode_ = scene_->CreateChild("camera");
    camNode_->CreateComponent<ProcGen::CameraController>();
    auto* camera = camNode_->CreateComponent<Camera>();

    auto* renderer = GetSubsystem<Renderer>();
    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, camNode_->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);

    auto* cache = GetSubsystem<ResourceCache>();
    SharedPtr<RenderPath> effectRenderPath = viewport->GetRenderPath()->Clone();
    // effectRenderPath->Append(cache->GetResource<XMLFile>("PostProcess/GreyScale.xml"));
    effectRenderPath->Append(cache->GetResource<XMLFile>("PostProcess/PP_Basic.xml"));
    // effectRenderPath->SetShaderParameter("BloomMix", Vector2(1.1f, .7f));
    // effectRenderPath->SetEnabled("Bloom", true);
    // viewport->SetRenderPath(effectRenderPath);
}

Scene* SceneManager::GetScene() {
    return scene_;
}
