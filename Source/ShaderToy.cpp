#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Graphics/TextureCube.h>

#include "ProcGen/Components/CameraController.h"
#include "ProcGen/Subsystems/ModelCreator.h"
#include "ProcGen/Subsystems/MaterialCreator.h"
#include "ProcGen/Subsystems/TextureCreator.h"
#include "ProcGen/Subsystems/SceneManager.h"

#include "ShaderToy.h"

using namespace Toy;

ShaderToy::ShaderToy(Context* context) : Object(context) {}

void ShaderToy::Start() {
    auto* cache = GetSubsystem<ResourceCache>();
    ProcGen::ModelCreator* modelCreator = GetSubsystem<ProcGen::ModelCreator>();
    ProcGen::TextureCreator* textureCreator =  GetSubsystem<ProcGen::TextureCreator>();
    ProcGen::MaterialCreator* materialCreator =  GetSubsystem<ProcGen::MaterialCreator>();
    auto* sceneManager = GetSubsystem<ProcGen::SceneManager>();

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

    // DIFFUSE > IMAGE
    // diffuseTexture = textureCreator->CreateImageTexture(procImg);
    // diffuseTexture = textureCreator->CreateImageTexture(mmDiffuseImg);

    // DIFFUSE > EFFECT (Post-process)
    diffuseTexture = textureCreator->CreateEffectTexture(w, h, "PPE_Shapes");
    // diffuseTexture = textureCreator->CreateEffectTexture(w, h, "PPE_ScottishTartan");
    // diffuseTexture = textureCreator->CreateEffectTexture(w, h, "PPE_Patterns_TicTacToe");
    // diffuseTexture = textureCreator->CreateEffectTexture(w, h, "PPE_Bricks");
    // diffuseTexture = textureCreator->CreateEffectTexture(w, h, "PPE_Truchet");
    
    // diffuseTexture->SetFilterMode(Urho3D::FILTER_NEAREST);
    diffuseTexture->SetFilterMode(Urho3D::FILTER_BILINEAR);
    
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
    material = materialCreator->Create("Tec_Basic", textureData);
    // material = materialCreator->Create("Tec_Basic_Lit", textureData);
    // material = materialCreator->Create("Tec_Basic_Rim", textureData);
    // material = materialCreator->Create("Tec_DiffUnlit", textureData);
    
    // ================================================================
    // NODE

    Node* node;
    
    node = modelCreator->CreateStockModel("Box", material);
    
    // node = modelCreator->CreateStockModel("Cylinder", material);
    
    // node = modelCreator->CreateStockModel("Pyramid", material);
    
    // node = modelCreator->CreateStockModel("Torus", material);

    // node = modelCreator->CreateStockModel("Sphere", material);

    // node = modelCreator->CreateStockModel("TeaPot", material);

    // node = modelCreator->CreateStockModel("Plane", material);
    // node->Rotate(Quaternion(-90, 0, 0));

    // ================================================================
    // BODY
    auto* body = node->CreateComponent<RigidBody>();
    body->SetMass(1);
    body->SetUseGravity(false);
    body->SetAngularRestThreshold(0);
    body->SetAngularVelocity(Vector3::UP * .5);

    // ================================================================
    // CAM
    auto* camNode = sceneManager->GetScene()->GetChild("Camera");
    
    camNode->Rotate(Quaternion(30, 180, 0));
    camNode->Translate(Vector3::BACK * 3);
    
    // camNode->Translate(Vector3::BACK * 2);
    
    camNode->GetComponent<ProcGen::CameraController>()->UpdateRotation();

    // ================================================================
    // SPRITE

    auto* ui = GetSubsystem<UI>();
    // auto* graphics = GetSubsystem<Graphics>();
    
    // auto width = (float)graphics->GetWidth();
    // auto height = (float)graphics->GetHeight();
    
    SharedPtr<Sprite> sprite(new Sprite(context_));
    sprite->SetTexture(diffuseTexture);
    sprite->SetSize(IntVector2(w, h));
    
    ui->GetRoot()->AddChild(sprite);        
}
