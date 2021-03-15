#include <Urho3D/Graphics/GraphicsDefs.h>
#include <Urho3D/Graphics/Texture.h>
#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Math/Color.h>
#include <Urho3D/Resource/Image.h>
#include <iostream>

#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Scene/ValueAnimation.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/IndexBuffer.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/UI/UI.h>

#include "App.h"

#include "Subsystems/SceneManager.h"
#include "Subsystems/ModelCreator.h"

#include "Components/CameraController.h"
#include "Components/ProcModel.h"

using namespace Urho3D;
App::App(Context* context) :
    Application(context) {
    context_->RegisterSubsystem<ProcGen::SceneManager>();
    context_->RegisterSubsystem<ProcGen::ModelCreator>();
    
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
    
    ProcGen::SceneManager* sceneManager = GetSubsystem<ProcGen::SceneManager>();
    sceneManager->CreateScene();
    sceneManager->SetupViewport();
    scene_ = sceneManager->GetScene();

    CreateTexture();
    CreateStockModel();
    // CreateProceduralModel();
}

void App::CreateTexture() {
    SharedPtr<Image> image(new Image(context_));
    SharedPtr<Texture2D> texture2D(new Texture2D(context_));
    int w = 320;
    int h = 320;
    image->SetSize(w, h, 3);
    texture2D->SetSize(w, h, Graphics::GetRGBFormat(), Urho3D::TEXTURE_DYNAMIC);

    for (int y = 0; y < h; ++y) {
        bool ymod = y % (w/10) < (w/20);
        for (int x = 0; x < w; ++x) {
            if (ymod) image->SetPixel(x, y, Color::RED);
            else image->SetPixel(x, y, Color::GREEN);
        }
    }
    texture2D->SetData(image);
    image->SavePNG("Data/Textures/Lines.png");
}

void App::CreateStockModel() {
    auto* cache = GetSubsystem<ResourceCache>();
    
    ProcGen::ModelCreator* modelCreator = GetSubsystem<ProcGen::ModelCreator>();
    modelCreator->Start();
    Material* material = cache->GetResource<Material>("Data/Materials/PG_Basic.xml");
    Node* node = modelCreator->CreateStockModel("Box", material);
    
    auto* body = node->CreateComponent<RigidBody>();
    body->SetMass(1);
    body->SetUseGravity(false);
    body->SetAngularVelocity(Vector3::UP * 1.5);
}

void App::CreateProceduralModel() {
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

    procModel->normals_ = {
        Vector3::BACK,Vector3::BACK,Vector3::BACK,Vector3::BACK,
        Vector3::UP,Vector3::UP,Vector3::UP,Vector3::UP,
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
    // mat1->SetTechnique(0, techNoTextureVCol);
    mat1->SetTechnique(0, techNoTexture);

    procModel->material_ = mat1;
    procModel->Generate();

    // node->Translate(Vector3::ONE);
    // node->Rotate(Quaternion(30, 30, 0));
    // node->Scale(1.5);
    
    // node = scene_->CreateChild("ProceduralObject");
    // object = node->CreateComponent<StaticModel>();
    // object->SetModel(model);
    // node->Translate(Vector3::LEFT);
}

void App::Stop() {
}

void App::SubscribeToEvents() {
    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(App, HandleKeyDown));    
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(App, HandleUpdate));
}

void App::HandleKeyDown(StringHash eventType, VariantMap& eventData) {
    using namespace KeyDown;
    int key = eventData[P_KEY].GetInt();
    if (key == KEY_ESCAPE)
        engine_->Exit();

    
    auto* node = scene_->GetChild("ProceduralObject");
    if (!node) return;
    auto* procModel = node->GetComponent<ProcGen::ProcModel>();
    if (key == KEY_M) {
        procModel->positions_[0] = procModel->positions_[0] * 1.25;
        procModel->Generate();
    }

    if (key == KEY_1) {
        procModel->SetDrawNormals(!procModel->GetDrawNormals());
    }
}

void App::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();
}

URHO3D_DEFINE_APPLICATION_MAIN(App)
