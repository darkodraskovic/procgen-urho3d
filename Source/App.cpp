#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Graphics/Texture.h>
#include <Urho3D/Input/Controls.h>
#include <Urho3D/Input/InputConstants.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Math/Quaternion.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/ProgressBar.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Engine/DebugHud.h>
#include <Urho3D/Engine/Console.h>
#include <Urho3D/Core/WorkQueue.h>
#include <Urho3D/UI/UIElement.h>

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
}

void App::Setup() {
    engineParameters_[EP_FULL_SCREEN]  = false;
    engineParameters_[EP_WINDOW_POSITION_X]  = 0;
    engineParameters_[EP_WINDOW_POSITION_Y]  = 0;
    engineParameters_[EP_WINDOW_WIDTH]  = 1366;
    engineParameters_[EP_WINDOW_HEIGHT]  = 768;
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

    CreateStockModel();
    // CreateProceduralModel();

    // CreateVoxels();
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

void App::CreateStockModel() {
    auto* cache = GetSubsystem<ResourceCache>();
    ProcGen::ModelCreator* modelCreator = GetSubsystem<ProcGen::ModelCreator>();
    ProcGen::TextureCreator* textureCreator =  GetSubsystem<ProcGen::TextureCreator>();
    ProcGen::MaterialCreator* materialCreator =  GetSubsystem<ProcGen::MaterialCreator>();

    int w = 320, h = 320;

    // ================================================================

    // IMAGE
    auto* mmDiffuseImg = cache->GetResource<Image>("Textures/MM_Diffuse.png");
    auto* mmNormalImg = cache->GetResource<Image>("Textures/MM_Normal.png");

    Image* procImg(new Image(context_));
    procImg->SetSize(w, h, 3);
    for (int y = 0; y < h; ++y) {
        bool ymod = y % (w/10) < (w/20);
        for (int x = 0; x < w; ++x) {
            if (ymod) procImg->SetPixel(x, y, Color::RED);
            else procImg->SetPixel(x, y, Color::GREEN);
        }
    }

    // ================================================================
    // TEXTURE
    
    Texture2D* diffuseTexture;

    // DIFFUSE
    // image based textures
    // diffuseTexture = textureCreator->CreateImageTexture(procImg);
    // diffuseTexture = textureCreator->CreateImageTexture(mmDiffuseImg);

    // effect based textures
    // diffuseTexture = textureCreator->CreateEffectTexture(w, h, "PP_Basic");
    diffuseTexture = textureCreator->CreateEffectTexture(w, h, "PP_ScottishTartan");
    // diffuseTexture = textureCreator->CreateEffectTexture(w, h, "PP_Patterns_TicTacToe");

    // NORMAL
    Texture2D* normalTexture;
    normalTexture = textureCreator->CreateImageTexture(mmNormalImg);

    // SKYBOX
    auto* skyboxTexture = cache->GetResource<TextureCube>("Textures/Space.xml");

    // ================================================================
    // MATERIAL
    
    HashMap<TextureUnit, Texture*> textureData = {
        {TU_DIFFUSE, diffuseTexture}, {TU_NORMAL, normalTexture}, {TU_ENVIRONMENT, skyboxTexture},
    };

    Material* material;
    material = materialCreator->Create("PG_DiffUnlit", textureData);
    
    // ================================================================
    // NODE

    Node* node;
    // node = modelCreator->CreateStockModel("Box", material);

    // node = modelCreator->CreateStockModel("Sphere", material);

    node = modelCreator->CreateStockModel("Plane", material);
    node->Rotate(Quaternion(-90, 0, 0));

    // node = modelCreator->CreateStockModel("TeaPot", material);

    // ================================================================
    // BODY
    auto* body = node->CreateComponent<RigidBody>();
    body->SetMass(1);
    body->SetUseGravity(false);
    // body->SetAngularVelocity(Vector3::UP * 1.5);

    // ================================================================
    // CAM
    auto* camNode = scene_->GetChild("Camera");
    // camNode->Rotate(Quaternion(30, 0, 0));
    camNode->Translate(Vector3::BACK * 2);
    camNode->GetComponent<ProcGen::CameraController>()->UpdateRotation();
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

void App::CreateVoxels() {
    auto* cache = GetSubsystem<ResourceCache>();
    auto* ui = GetSubsystem<UI>();
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
    auto* cam = scene_->GetChild("Camera");

    world->SetRoot(scene_);

    ProcGen::TextureCreator* textureCreator =  GetSubsystem<ProcGen::TextureCreator>();

    // material
    auto* image = cache->GetResource<Image>("Textures/VoxelTerrain.png");
    auto* texture = textureCreator->CreateImageTexture(image);
    texture->SetFilterMode(Urho3D::FILTER_NEAREST);

    auto* techDiff = cache->GetResource<Technique>("CoreData/Techniques/Diff.xml");
    // auto* techNoTexture = cache->GetResource<Technique>("CoreData/Techniques/NoTexture.xml");
    // auto* techNoTextureVCol = cache->GetResource<Technique>("CoreData/Techniques/NoTextureVCol.xml");
    // auto* techDiffLightMap = cache->GetResource<Technique>("CoreData/Techniques/DiffLightMap.xml");
    // auto* techDiffAO = cache->GetResource<Technique>("CoreData/Techniques/DiffAO.xml");
    auto* material = new Material(context_);
    material->SetTechnique(0, techDiff);
    material->SetTexture(Urho3D::TU_DIFFUSE, texture);
    world->SetMaterial(material);

    world->size_ = {8, 4, 8};

    world->CreateColumns();
    world->Build();
    // world->Model();

    Vector3 size = world->GetWorldSize();
    cam->SetPosition(Vector3::UP * size.y_ + Vector3::RIGHT * size.x_);
    cam->LookAt(size / 2);
    cam->Translate(Vector3::BACK * size.y_);
    cam->GetComponent<ProcGen::CameraController>()->UpdateRotation();

    CreateCharacter();
    world->SetPlayer(player_);
    player_->GetNode()->SetPosition(size / 2 + Vector3::UP * size.y_ / 2);
}

void App::CreateCharacter() {
    Node* node = scene_->CreateChild("Player");

    Node* lightNode = node->CreateChild("DirectionalLight");
    lightNode->Translate(Vector3::UP * 2);
    auto* light = lightNode->CreateComponent<Light>();
    light->SetLightType(Urho3D::LIGHT_POINT);
    light->SetBrightness(.5);

    auto* body = node->CreateComponent<RigidBody>();
    body->SetCollisionLayer(1);
    body->SetMass(1.0f);
    body->SetAngularFactor(Vector3::ZERO);
    body->SetCollisionEventMode(COLLISION_ALWAYS);

    auto* shape = node->CreateComponent<CollisionShape>();
    shape->SetCapsule(0.7f, 1.8f, Vector3(0.0f, 0.9f, 0.0f));
    player_ = node->CreateComponent<Voxels::Character>();
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

    if (key == KEY_TAB) {
        firstPerson_ = !firstPerson_;
        auto* camController = scene_->GetChild("Camera")->GetComponent<ProcGen::CameraController>();
        camController->SetEnabled(!firstPerson_);
        if (!firstPerson_) {
            camController->UpdateRotation();
        }
    }
}

void App::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    auto* input = GetSubsystem<Input>();
    Controls& controls = player_ && firstPerson_ ? player_->controls_ :
        scene_->GetChild("Camera")->GetComponent<ProcGen::CameraController>()->controls_;

    using namespace ProcGen;
    controls.Set(CTRL_FORWARD, input->GetKeyDown(KEY_W));
    controls.Set(CTRL_BACK, input->GetKeyDown(KEY_S));
    controls.Set(CTRL_LEFT, input->GetKeyDown(KEY_A));
    controls.Set(CTRL_RIGHT, input->GetKeyDown(KEY_D));

    controls.Set(CTRL_DOWN, input->GetKeyDown(KEY_Q));
    controls.Set(CTRL_UP, input->GetKeyDown(KEY_E));

    controls.Set(CTRL_JUMP, input->GetKeyDown(KEY_SPACE));

    float YAW_SENSITIVITY = 0.1f;
    controls.yaw_ += (float)input->GetMouseMoveX() * YAW_SENSITIVITY;
    controls.pitch_ += (float)input->GetMouseMoveY() * YAW_SENSITIVITY;
}

void App::HandlePostUpdate(StringHash eventType, VariantMap &eventData) {
    if (player_ && firstPerson_) {
        Node* playerNode = player_->GetNode();

        // Get camera lookat dir from character yaw + pitch
        const Quaternion& rot = playerNode->GetRotation();
        Quaternion dir = rot * Quaternion(player_->controls_.pitch_, Vector3::RIGHT);

        auto* camNode = scene_->GetChild("Camera");
        camNode->SetRotation(dir);
        Vector3 pos = playerNode->GetPosition(); pos.y_ += player_->GetSize().y_;
        camNode->SetPosition(pos);
    }
}

// void App::HandlePostrenderupdate(StringHash eventType, VariantMap& eventData) {
// }

URHO3D_DEFINE_APPLICATION_MAIN(App)
