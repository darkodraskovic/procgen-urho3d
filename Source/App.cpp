#include <Urho3D/Core/Object.h>
#include <Urho3D/Graphics/GraphicsDefs.h>
#include <Urho3D/Graphics/Texture.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/IO/Log.h>

#include "App.h"

#include "Subsystems/SceneManager.h"
#include "Subsystems/ModelCreator.h"
#include "Subsystems/TextureCreator.h"
#include "Subsystems/MaterialCreator.h"

#include "Components/CameraController.h"
#include "Components/ProcModel.h"

using namespace Urho3D;
App::App(Context* context) :
    Application(context) {
    context_->RegisterSubsystem<ProcGen::SceneManager>();
    context_->RegisterSubsystem<ProcGen::ModelCreator>();
    context_->RegisterSubsystem<ProcGen::TextureCreator>();
    context_->RegisterSubsystem<ProcGen::MaterialCreator>();
    
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

    ProcGen::ModelCreator* modelCreator = GetSubsystem<ProcGen::ModelCreator>();
    modelCreator->Start();

    GetSubsystem<ProcGen::TextureCreator>()->Start();

    GetSubsystem<ProcGen::MaterialCreator>()->Start();
    
    CreateStockModel();
    // CreateProceduralModel();
}

void App::CreateStockModel() {
    auto* cache = GetSubsystem<ResourceCache>();
    ProcGen::ModelCreator* modelCreator = GetSubsystem<ProcGen::ModelCreator>();
    ProcGen::TextureCreator* textureCreator =  GetSubsystem<ProcGen::TextureCreator>();
    ProcGen::MaterialCreator* materialCreator =  GetSubsystem<ProcGen::MaterialCreator>();

    // TEXTURE
    
    int w = 320, h = 320;
    // texture_ = textureCreator->CreateEffectTexture(w, h, "PP_Basic");
    texture_ = textureCreator->CreateEffectTexture(w, h, "PP_ScottishTartan");
    // texture_ = textureCreator->CreateEffectTexture(w, h, "PP_Patterns_TicTacToe");

    // Image* image(new Image(context_));
    // image->SetSize(w, h, 3);
    // for (int y = 0; y < h; ++y) {
    //     bool ymod = y % (w/10) < (w/20);
    //     for (int x = 0; x < w; ++x) {
    //         if (ymod) image->SetPixel(x, y, Color::RED);
    //         else image->SetPixel(x, y, Color::GREEN);
    //     }
    // }
    // texture_ = textureCreator->CreateImageTexture(image);

    // MATERIAL
    
    // Material* material = materialCreator->Create("Unlit", Color::WHITE, texture_);
    Material* material = materialCreator->Create("PG_Basic00", Color::WHITE, texture_);

    // material->SetTexture(TU_DIFFUSE, texture_);

    // NODE
    
    // Node* node = modelCreator->CreateStockModel("Box", material);
    
    // Node* node = modelCreator->CreateStockModel("Plane", material);
    // node->Rotate(Quaternion(-90, 0, 0));

    Node* node = modelCreator->CreateStockModel("TeaPot", material);
    
    // BODY
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
    // SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(App, HandlePostrenderupdate));
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

// void App::HandlePostrenderupdate(StringHash eventType, VariantMap& eventData) {
//     if (texture_) {
//         texture_->GetImage()->SavePNG("Data/Textures/Scottish.png");
//         texture_ = nullptr;
//     }
// }

URHO3D_DEFINE_APPLICATION_MAIN(App)
