#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/UI/ProgressBar.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Graphics/Technique.h>

#include "ProcGen/Components/CameraController.h"
#include "ProcGen/Subsystems/SceneManager.h"
#include "ProcGen/Subsystems/TextureCreator.h"

#include "Voxels/Subsystems/Utils.h"
#include "Voxels/Subsystems/World.h"

#include "VoxelToy.h"
#include "App.h"
#include "Controller.h"

using namespace Toy;

VoxelToy::VoxelToy(Context *context) : Object(context) {}

void VoxelToy::Start() {
    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(VoxelToy, HandleKeyDown));
    SubscribeToEvent(E_POSTUPDATE, URHO3D_HANDLER(VoxelToy, HandlePostUpdate));
}

void VoxelToy::CreateSceneContent() {
    CreateVoxels();
    CreateCharacter();
}

void VoxelToy::CreateVoxels() {
    auto* cache = GetSubsystem<ResourceCache>();
    auto* ui = GetSubsystem<UI>();

    auto* sceneManager = GetSubsystem<ProcGen::SceneManager>();
    
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
    auto* cam = sceneManager->GetScene()->GetChild("Camera");

    world->SetRoot(sceneManager->GetScene());

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
}

void VoxelToy::CreateCharacter() {
    auto* sceneManager = GetSubsystem<ProcGen::SceneManager>();
    
    Node* node = sceneManager->GetScene()->CreateChild("Player");

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

    auto* world = GetSubsystem<Voxels::World>();    
    world->SetPlayer(player_);
    Vector3 size = world->GetWorldSize();    
    player_->GetNode()->SetPosition(size / 2 + Vector3::UP * size.y_ / 2);
}

void VoxelToy::HandleKeyDown(StringHash eventType, VariantMap& eventData) {
    using namespace KeyDown;
    int key = eventData[P_KEY].GetInt();
    auto* scene = GetSubsystem<ProcGen::SceneManager>()->GetScene();

    if (key == KEY_TAB) {
        firstPerson_ = !firstPerson_;
        auto* camController = scene->GetChild("Camera")->GetComponent<ProcGen::CameraController>();
        camController->SetEnabled(!firstPerson_);
        Urho3D::Controls* ctrl = firstPerson_ ? &player_->controls_ : &camController->controls_;
        GetSubsystem<ProcGen::Controller>()->SetControls(ctrl);
        if (!firstPerson_) {
            camController->UpdateRotation();
        } 
    }
}

void VoxelToy::HandlePostUpdate(StringHash eventType, VariantMap &eventData) {
    if (player_ && firstPerson_) {
        Node* playerNode = player_->GetNode();

        // Get camera lookat dir from character yaw + pitch
        const Quaternion& rot = playerNode->GetRotation();
        Quaternion dir = rot * Quaternion(player_->controls_.pitch_, Vector3::RIGHT);

        auto* camNode = GetSubsystem<ProcGen::SceneManager>()->GetScene()->GetChild("Camera");
        camNode->SetRotation(dir);
        Vector3 pos = playerNode->GetPosition(); pos.y_ += player_->GetSize().y_;
        camNode->SetPosition(pos);
    }
}
