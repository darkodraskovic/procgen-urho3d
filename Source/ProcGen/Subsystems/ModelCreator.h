#pragma once

#include <Urho3D/Urho3DAll.h>

namespace ProcGen {

class ModelCreator : public Object {
  URHO3D_OBJECT(ModelCreator, Object);

 public:
  explicit ModelCreator(Context* context);
  void Start();
  Node* CreateStockModel(const String& modelName, Material* material = nullptr,
                         const Vector3& position = Vector3::ZERO,
                         const Quaternion& rotation = Quaternion::IDENTITY);

 private:
  SharedPtr<Material> defaultMaterial_;
};
}  // namespace ProcGen
