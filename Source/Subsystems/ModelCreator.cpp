#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Model.h>

#include "ModelCreator.h"
#include "SceneManager.h"

using namespace ProcGen;

ModelCreator::ModelCreator(Context *context) : Object(context) {}

void ModelCreator::Start() {
    scene_ = GetSubsystem<ProcGen::SceneManager>()->GetScene();

    auto* cache = GetSubsystem<ResourceCache>();
    
    auto* tech = cache->GetResource<Technique>("Techniques/NoTexture.xml");
    defaultMaterial_ = new Material(context_);
    defaultMaterial_->SetTechnique(0, tech);
}

Node* ModelCreator::CreateStockModel(const String& modelName, Material* material){
    auto* cache = GetSubsystem<ResourceCache>();
    
    Node* node = scene_->CreateChild(modelName);
    auto* staticModel = node->CreateComponent<StaticModel>();
    staticModel->SetModel(cache->GetResource<Model>("Models/" + modelName + ".mdl"));

    if (!material) staticModel->SetMaterial(defaultMaterial_);
    else staticModel->SetMaterial(material);
    
    return node;
}
