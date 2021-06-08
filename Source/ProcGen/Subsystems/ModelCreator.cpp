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
    auto* cache = GetSubsystem<ResourceCache>();
    auto* tech = GetSubsystem<ResourceCache>()->GetResource<Technique>("Techniques/NoTexture.xml");
    defaultMaterial_ = new Material(context_);
    defaultMaterial_->SetTechnique(0, tech);
}

Node* ModelCreator::CreateStockModel(const String& modelName, Material* material){
    Scene* scene = GetSubsystem<ProcGen::SceneManager>()->GetScene();
    
    Node* node = scene->CreateChild(modelName);
    StaticModel* staticModel = node->CreateComponent<StaticModel>();
    auto* cache = GetSubsystem<ResourceCache>();
    staticModel->SetModel(cache->GetResource<Model>("Models/" + modelName + ".mdl"));

    if (!material) staticModel->SetMaterial(defaultMaterial_);
    else staticModel->SetMaterial(material);

    return node;
}
