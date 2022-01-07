#pragma once

#include <Urho3D/Urho3DAll.h>

namespace Toy {

class ShaderToy : public Object {
  URHO3D_OBJECT(ShaderToy, Object);

 public:
  explicit ShaderToy(Context* context);

  void Start();

 private:
  Scene* scene_;
};
}  // namespace Toy
