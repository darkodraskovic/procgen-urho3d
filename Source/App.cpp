#include <Urho3D/Math/Color.h>
#include <Urho3D/Math/Quaternion.h>
#include <iostream>

#include <Urho3D/Graphics/GraphicsDefs.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/IndexBuffer.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/UI/UI.h>
#include <sys/types.h>

#include "App.h"
#include "CameraController.h"
#include "ProcModel.h"

using namespace Urho3D;
App::App(Context* context) :
    Application(context) {

    context_->RegisterFactory<ProcGen::CameraController>();
    context_->RegisterFactory<ProcGen::ProcModel>();
}

void App::Setup() {
    engineParameters_[EP_FULL_SCREEN]  = false;
    engineParameters_[EP_WINDOW_POSITION_X]  = 0;
    engineParameters_[EP_WINDOW_POSITION_Y]  = 0;
    engineParameters_[EP_WINDOW_WIDTH]  = 1024;
    engineParameters_[EP_WINDOW_HEIGHT]  = 640;    
}

void App::Start() {
    SubscribeToEvents();
    CreateScene();
    SetupViewport();

    Node* node = scene_->CreateChild("ProceduralObject");
    ProcGen::ProcModel* procModel = node->CreateComponent<ProcGen::ProcModel>();
    
    procModel->positions_ = {
        {-1.0f, -1.0f, 0.0f},
        {-1.0f, 1.0f, 0.0f},
        {1.0f, 1.0f, 0.0f},
        {1.0f, -1.0f, 0.0f},

        {-1.0f, 1.0f, 0.0f},
        {-1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 0.0f},
    };
    
    procModel->colors_ = {
        255,0,0,255, 0,255,0,255, 0,0,255,255, 255,0,255,255,
        255,0,0,255, 0,255,0,255, 0,0,255,255, 255,0,255,255,
    };

    procModel->indices_ = {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7,
    };

    auto* cache = GetSubsystem<ResourceCache>();
    
    auto* techNoTexture = cache->GetResource<Technique>("Techniques/NoTexture.xml");
    auto* techNoTextureUnlit = cache->GetResource<Technique>("Techniques/NoTextureUnlit.xml");
    auto* techNoTextureUnlitAlpha = cache->GetResource<Technique>("Techniques/NoTextureUnlitAlpha.xml");
    auto* techNoTextureVCol = cache->GetResource<Technique>("Techniques/NoTextureVCol.xml");
    auto* mat1 = new Material(context_);
    mat1->SetTechnique(0, techNoTextureVCol);

    procModel->material_ = mat1;
    procModel->Generate();

    node->Translate(Vector3::ONE);
    // node->Rotate(Quaternion(30, 30, 0));
    node->Scale(1.5);
    
    // node = scene_->CreateChild("ProceduralObject");
    // object = node->CreateComponent<StaticModel>();
    // object->SetModel(model);
    // node->Translate(Vector3::LEFT);
}

void App::SubscribeToEvents() {
    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(App, HandleKeyDown));    
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(App, HandleUpdate));
    // SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(App, HandlePostRenderUpdate));    
}

void App::CreateScene() {
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

void App::SetupViewport() {
    cameraNode_ = scene_->CreateChild("camera");
    cameraNode_->SetPosition(Vector3(0.0f, 4.0f, -8.0f));
    cameraNode_->LookAt(Vector3::ZERO);
    cameraNode_->CreateComponent<ProcGen::CameraController>();
    auto* camera = cameraNode_->CreateComponent<Camera>();
    
    auto* renderer = GetSubsystem<Renderer>();
    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);    
}

void App::Stop() {
}

void App::HandleKeyDown(StringHash eventType, VariantMap& eventData) {
    using namespace KeyDown;
    int key = eventData[P_KEY].GetInt();
    if (key == KEY_ESCAPE)
        engine_->Exit();

    auto* node = scene_->GetChild("ProceduralObject");
    auto* procModel = node->GetComponent<ProcGen::ProcModel>();
    if (key == KEY_M) {
        procModel->positions_[0] = procModel->positions_[0] * 1.25;
        procModel->Generate();
    }

    if (key == KEY_P) {
        procModel->SetDrawNormals(true);
    }
    else if (key == KEY_O) {
        procModel->SetDrawNormals(false);
    }
}

void App::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();
}

void App::HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData)
{
    debugRenderer_->AddLine(Vector3::ZERO, Vector3::RIGHT, Color::WHITE);
}

URHO3D_DEFINE_APPLICATION_MAIN(App)
