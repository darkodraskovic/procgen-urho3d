#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Engine/DebugHud.h>
#include <Urho3D/Engine/Console.h>

#include "App.h"

#include "ProcGen/Subsystems/SceneManager.h"
#include "ProcGen/Subsystems/ModelCreator.h"
#include "ProcGen/Subsystems/TextureCreator.h"
#include "ProcGen/Subsystems/MaterialCreator.h"

#include "ProcGen/Components/CameraController.h"
#include "ProcGen/Components/ProcModel.h"

#include "Voxels/Subsystems/World.h"
#include "Voxels/Subsystems/Utils.h"
#include "Voxels/Components/Block.h"
#include "Voxels/Components/Chunk.h"
#include "Voxels/Components/Character.h"

#include "ShaderToy.h"
#include "VoxelToy.h"
#include "Controller.h"

using namespace Urho3D;
App::App(Context* context) :
    Application(context) {
    context_->RegisterSubsystem<ProcGen::SceneManager>();
    context_->RegisterSubsystem<ProcGen::ModelCreator>();
    context_->RegisterSubsystem<ProcGen::TextureCreator>();
    context_->RegisterSubsystem<ProcGen::MaterialCreator>();

    context_->RegisterFactory<ProcGen::CameraController>();
    context_->RegisterFactory<ProcGen::ProcModel>();

    context_->RegisterSubsystem<Voxels::Utils>();
    context_->RegisterSubsystem<Voxels::World>();

    context_->RegisterFactory<Voxels::Block>();
    context_->RegisterFactory<Voxels::Chunk>();
    Voxels::Character::RegisterObject(context);


    context_->RegisterSubsystem<ProcGen::Controller>();
    
    context_->RegisterSubsystem<Toy::ShaderToy>();
    context_->RegisterSubsystem<Toy::VoxelToy>();
}

void App::Setup() {
    engineParameters_[EP_FULL_SCREEN]  = false;
    engineParameters_[EP_WINDOW_POSITION_X]  = 0;
    engineParameters_[EP_WINDOW_POSITION_Y]  = 0;
    engineParameters_[EP_WINDOW_WIDTH]  = 1366;
    engineParameters_[EP_WINDOW_HEIGHT]  = 768;
    engineParameters_[EP_MULTI_SAMPLE] = 4;    
}

void threadHello(const WorkItem *workItem, unsigned threadIndex) {
    URHO3D_LOGINFOF("I'm executing on thread %d", threadIndex); // debug
}

void App::Start() {
    CreateConsoleAndDebugHud();
    SubscribeToEvents();

    ProcGen::SceneManager* sceneManager = GetSubsystem<ProcGen::SceneManager>();
    sceneManager->CreateScene();
    sceneManager->CreateSkybox("Materials/Space.xml");
    sceneManager->SetupViewport();
    scene_ = sceneManager->GetScene();

    ProcGen::ModelCreator* modelCreator = GetSubsystem<ProcGen::ModelCreator>();
    modelCreator->Start();

    GetSubsystem<ProcGen::TextureCreator>()->Start();
    GetSubsystem<ProcGen::MaterialCreator>()->Start();

    GetSubsystem<Voxels::World>()->Start();

    // CONTROLLER
    
    GetSubsystem<ProcGen::Controller>()->Start();
    GetSubsystem<ProcGen::Controller>()->SetControls(&scene_->GetChild("Camera")->GetComponent<ProcGen::CameraController>()->controls_);

    // SHADER
    
    GetSubsystem<Toy::ShaderToy>()->CreateSceneContent();

    // VOXEL
    
    // GetSubsystem<Toy::VoxelToy>()->Start();
    // GetSubsystem<Toy::VoxelToy>()->CreateSceneContent();

    // PROC MODEL
    
    // CreateProceduralModel();
}

void App::CreateConsoleAndDebugHud() {
    // Get default style
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    XMLFile* xmlFile = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");

    // Create console
    Console* console = engine_->CreateConsole();
    console->SetDefaultStyle(xmlFile);
    console->GetBackground()->SetOpacity(0.8f);

    // Create debug HUD.
    DebugHud* debugHud = engine_->CreateDebugHud();
    debugHud->SetDefaultStyle(xmlFile);
}

void App::CreateProceduralModel() {
    Node* node = scene_->CreateChild("ProceduralObject");
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
    for (auto& p : points) p *= .5; // [-.5, .5]


    procModel->positions_ = {
        points[0], points[1], points[2], points[3], // back
        points[0], points[4], points[7], points[1], // left
        points[4], points[5], points[6], points[7], // front
        points[3], points[2], points[6], points[5], // right
        points[0], points[3], points[5], points[4], // bottom
        points[1], points[7], points[6], points[2], // up
    };

    procModel->normals_ = {
        Vector3::BACK,Vector3::BACK,Vector3::BACK,Vector3::BACK,
        Vector3::LEFT,Vector3::LEFT,Vector3::LEFT,Vector3::LEFT,
        Vector3::FORWARD,Vector3::FORWARD,Vector3::FORWARD,Vector3::FORWARD,
        Vector3::RIGHT,Vector3::RIGHT,Vector3::RIGHT,Vector3::RIGHT,
        Vector3::DOWN,Vector3::DOWN,Vector3::DOWN,Vector3::DOWN,
        Vector3::UP,Vector3::UP,Vector3::UP,Vector3::UP,
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
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23,
    };

    auto* cache = GetSubsystem<ResourceCache>();

    // technique
    auto* techNoTexture = cache->GetResource<Technique>("Techniques/NoTexture.xml");
    auto* techNoTextureUnlit = cache->GetResource<Technique>("Techniques/NoTextureUnlit.xml");
    auto* techNoTextureUnlitAlpha = cache->GetResource<Technique>("Techniques/NoTextureUnlitAlpha.xml");
    auto* techNoTextureVCol = cache->GetResource<Technique>("CoreData/Techniques/NoTextureVCol.xml");

    // material
    auto* mat1 = new Material(context_);
    // mat1->SetTechnique(0, techNoTextureVCol);
    mat1->SetTechnique(0, techNoTextureVCol);

    procModel->material_ = mat1;
    procModel->GenerateData();

    auto* body = node->CreateComponent<RigidBody>();
    body->SetMass(1);
    body->SetUseGravity(false);
    body->SetAngularVelocity(Vector3::ONE);

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
    SubscribeToEvent(E_POSTUPDATE, URHO3D_HANDLER(App, HandlePostUpdate));
    // SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(App, HandlePostrenderupdate));
}

void App::HandleKeyDown(StringHash eventType, VariantMap& eventData) {
    using namespace KeyDown;
    int key = eventData[P_KEY].GetInt();
    if (key == KEY_ESCAPE)
        engine_->Exit();

    // Toggle console with F1
    if (key == KEY_F1)
        GetSubsystem<Console>()->Toggle();

    // Toggle debug HUD with F2
    else if (key == KEY_F2)
        GetSubsystem<DebugHud>()->ToggleAll();


    auto* node = scene_->GetChild("ProceduralObject");
    if (node) {
        auto* procModel = node->GetComponent<ProcGen::ProcModel>();
        if (key == KEY_M) {
            procModel->positions_[0] = procModel->positions_[0] * 1.25;
            procModel->GenerateData();
        }

        if (key == KEY_1) {
            procModel->SetDrawNormals(!procModel->GetDrawNormals());
        }
    };
}

void App::HandleUpdate(StringHash eventType, VariantMap& eventData) {

}

void App::HandlePostUpdate(StringHash eventType, VariantMap &eventData) {
}

URHO3D_DEFINE_APPLICATION_MAIN(App)
