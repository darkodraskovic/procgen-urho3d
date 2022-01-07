#include "ModelCreator.h"

using namespace ProcGen;

ModelCreator::ModelCreator(Context* context) : Object(context) {}

void ModelCreator::Start() {
  auto* cache = GetSubsystem<ResourceCache>();
  auto* tech = GetSubsystem<ResourceCache>()->GetResource<Technique>(
      "Techniques/NoTexture.xml");
  defaultMaterial_ = new Material(context_);
  defaultMaterial_->SetTechnique(0, tech);
}

Node* ModelCreator::CreateStockModel(const String& modelName,
                                     Material* material,
                                     const Vector3& position,
                                     const Quaternion& rotation) {
  Node* node = new Node(context_);
  node->SetName(modelName);

  StaticModel* staticModel = node->CreateComponent<StaticModel>();
  auto* cache = GetSubsystem<ResourceCache>();
  staticModel->SetModel(
      cache->GetResource<Model>("Models/" + modelName + ".mdl"));

  if (!material)
    staticModel->SetMaterial(defaultMaterial_);
  else
    staticModel->SetMaterial(material);

  node->SetPosition(position);
  node->SetRotation(rotation);

  return node;
}
