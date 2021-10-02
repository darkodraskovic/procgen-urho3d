#include <Urho3D/Graphics/GraphicsDefs.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Graphics/TextureCube.h>

#include "../ProcGen/Components/CameraController.h"
#include "../ProcGen/Subsystems/ModelCreator.h"
#include "../ProcGen/Subsystems/MaterialCreator.h"
#include "../ProcGen/Subsystems/TextureCreator.h"
#include "../ProcGen/Subsystems/SceneManager.h"

#include "ShaderToy.h"

using namespace Toy;

ShaderToy::ShaderToy(Context* context) : Object(context) {}

void ShaderToy::Start() {
    auto* cache = GetSubsystem<ResourceCache>();
    ProcGen::ModelCreator* modelCreator = GetSubsystem<ProcGen::ModelCreator>();
    ProcGen::TextureCreator* textureCreator =  GetSubsystem<ProcGen::TextureCreator>();
    ProcGen::MaterialCreator* materialCreator =  GetSubsystem<ProcGen::MaterialCreator>();
    scene_ = GetSubsystem<ProcGen::SceneManager>()->GetScene();

    int w = 320*2, h = 320*2;

    // ================================================================
    // IMAGE

    // DIFFUSE
    Image* diffuseImage;
    
    diffuseImage = cache->GetResource<Image>("Textures/MM_Diffuse.png");

    // diffImage = new Image(context_);
    // diffImage->SetSize(w, h, 3);
    // for (int y = 0; y < h; ++y) {
    //     bool ymod = y % (w/10) < (w/20);
    //     for (int x = 0; x < w; ++x) {
    //         if (ymod) diffImage->SetPixel(x, y, Color::RED);
    //         else diffImage->SetPixel(x, y, Color::GREEN);
    //     }
    // }

    // NORMAL
    Image* normalImage;
    
    normalImage = cache->GetResource<Image>("Textures/MM_Normal.png");
    
    // ================================================================
    // TEXTURE
    
    Texture2D* diffuseTexture;

    // DIFFUSE > IMAGE
    // diffuseTexture = textureCreator->CreateImageTexture(diffuseImage);

    // DIFFUSE > EFFECT (Post-process)
    // diffuseTexture = textureCreator->CreateEffectTexture(w, h, "PPE_Shapes");
    // diffuseTexture = textureCreator->CreateEffectTexture(w, h, "PPE_ScottishTartan");
    // diffuseTexture = textureCreator->CreateEffectTexture(w, h, "PPE_Patterns_TicTacToe");
    // diffuseTexture = textureCreator->CreateEffectTexture(w, h, "PPE_Bricks");
    diffuseTexture = textureCreator->CreateEffectTexture(w, h, "PPE_Truchet");
    
    diffuseTexture->SetFilterMode(Urho3D::FILTER_BILINEAR);
    
    // NORMAL
    Texture2D* normalTexture;
    normalTexture = textureCreator->CreateImageTexture(normalImage);

    // SKYBOX
    TextureCube* skyboxTexture;
    skyboxTexture = cache->GetResource<TextureCube>("Textures/Space.xml");

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

    // node = modelCreator->CreateStockModel("Plane", material, Vector3::ZERO, Quaternion(90, 0, 0));

    scene_->AddChild(node);
    
    // ================================================================
    // BODY
    auto* body = node->CreateComponent<RigidBody>();
    body->SetMass(1);
    body->SetUseGravity(false);
    body->SetAngularRestThreshold(0);
    body->SetAngularVelocity(Vector3::UP * .5);

    // ================================================================
    // CAM
    auto* camNode = scene_->GetChild("Camera");

    camNode->Translate(Vector3::RIGHT);
    camNode->Rotate(Quaternion(30, 180, 0));
    camNode->Translate(Vector3::BACK * 3);
    camNode->GetComponent<ProcGen::CameraController>()->Sync();

    // ================================================================
    // Texture SPRITE

    auto* ui = GetSubsystem<UI>();
    
    SharedPtr<Sprite> sprite(new Sprite(context_));
    sprite->SetTexture(diffuseTexture);
    sprite->SetSize(IntVector2(w, h));
    
    ui->GetRoot()->AddChild(sprite);        
}
