#pragma once

#include <Urho3D/Urho3DAll.h>

namespace Toy {

class ProcModelToy : public Object {
  URHO3D_OBJECT(ProcModelToy, Object);

 public:
  explicit ProcModelToy(Context* context);
  Node* CreateModel();

  void Start();
};
}  // namespace Toy
