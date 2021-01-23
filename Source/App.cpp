#include <Urho3D/Graphics/GraphicsDefs.h>
#include <iostream>

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
#include <Urho3D/Input/Input.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/UI/UI.h>

#include "App.h"
#include "ModelGenerator.h"

using namespace Urho3D;
App::App(Context* context) :
    Application(context) {

    // context_->RegisterFactory<ModelGenerator>();
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

    float positions[] = {
        -1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
    };

    float normals[] = {
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        
    };

    unsigned short indices[] = {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7,
    };
    
    const unsigned numVertices = 8;
    const unsigned numIndices = 12;
    
    auto* modelGenerator = new ProcGen::ModelGenerator(context_);
    modelGenerator->SetVertexBuffer(positions, VertexMask::MASK_POSITION, numVertices);
    modelGenerator->SetVertexBuffer(normals, VertexMask::MASK_NORMAL);
    modelGenerator->SetIndices(indices, numIndices);
    auto* model = modelGenerator->Generate();
    
    Node* node = scene_->CreateChild("ProceduralObject");
    auto* object = node->CreateComponent<StaticModel>();
    object->SetModel(model);

    // node = scene_->CreateChild("ProceduralObject");
    // object = node->CreateComponent<StaticModel>();
    // object->SetModel(model);
    // node->Translate(Vector3::RIGHT);
    
    // node = scene_->CreateChild("ProceduralObject");
    // object = node->CreateComponent<StaticModel>();
    // object->SetModel(model);
    // node->Translate(Vector3::LEFT);
}

void App::SubscribeToEvents() {
    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(App, HandleKeyDown));    
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(App, HandleUpdate));
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
    // lightNode->SetDirection(Vector3(-0.6f, -1.0f, -0.8f));
    lightNode->SetDirection(Vector3(0, -1.0f, 0.8f));
    auto* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
    light->SetColor(Color(0.4f, 1.0f, 0.4f));
    light->SetSpecularIntensity(1.5f);
}

void App::SetupViewport() {
    cameraNode_ = new Node(context_);
    cameraNode_->SetPosition(Vector3(0.0f, 0.0f, -10.0f));
    cameraNode_->LookAt(Vector3::ZERO);
    auto* camera = cameraNode_->CreateComponent<Camera>();
    // camera->SetFarClip(300.0f);

    auto* renderer = GetSubsystem<Renderer>();
    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);    
}

void App::Stop() {
}

void App::MoveCamera(float timeStep) {
    // Do not move if the UI has a focused element (the console)
    if (GetSubsystem<UI>()->GetFocusElement())
        return;

    auto* input = GetSubsystem<Input>();


    // // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
    // IntVector2 mouseMove = input->GetMouseMove();
    // yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
    // pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
    // pitch_ = Clamp(pitch_, -90.0f, 90.0f);

    // // Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
    // cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));

    const float MOVE_SPEED = 16.0f;
    
    if (input->GetKeyDown(KEY_W))
        cameraNode_->Translate(Vector3::FORWARD * MOVE_SPEED * timeStep);
    if (input->GetKeyDown(KEY_S))
        cameraNode_->Translate(Vector3::BACK * MOVE_SPEED * timeStep);
    if (input->GetKeyDown(KEY_A))
        cameraNode_->Translate(Vector3::LEFT * MOVE_SPEED * timeStep);
    if (input->GetKeyDown(KEY_D))
        cameraNode_->Translate(Vector3::RIGHT * MOVE_SPEED * timeStep);
    if (input->GetKeyDown(KEY_Q))
        cameraNode_->Translate(Vector3::DOWN * MOVE_SPEED * timeStep);
    if (input->GetKeyDown(KEY_E))
        cameraNode_->Translate(Vector3::UP * MOVE_SPEED * timeStep);

    const float MOUSE_SENSITIVITY = 0.3f;
    
    if (input->GetKeyDown(KEY_J)) {
        cameraNode_->Rotate(Quaternion(0, -MOUSE_SENSITIVITY, 0));
    }
    if (input->GetKeyDown(KEY_L)) {
        cameraNode_->Rotate(Quaternion(0, MOUSE_SENSITIVITY, 0));
    }
    if (input->GetKeyDown(KEY_I)) {
        cameraNode_->Rotate(Quaternion(-MOUSE_SENSITIVITY, 0, 0));
    }
    if (input->GetKeyDown(KEY_K)) {
        cameraNode_->Rotate(Quaternion(MOUSE_SENSITIVITY, 0, 0));
    }
}

void App::HandleKeyDown(StringHash eventType, VariantMap& eventData) {
    using namespace KeyDown;
    int key = eventData[P_KEY].GetInt();
    if (key == KEY_ESCAPE)
        engine_->Exit();
}

void App::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    // Move the camera, scale movement with time step
    MoveCamera(timeStep);
}


URHO3D_DEFINE_APPLICATION_MAIN(App)
