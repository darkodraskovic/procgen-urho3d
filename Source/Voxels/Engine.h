#pragma once

#include "Components/Block.h"
#include "Components/Chunk.h"
#include "Subsystems/Utils.h"
#include "Subsystems/World.h"

using namespace Urho3D;

namespace Voxels {

class Engine : public Object {
  URHO3D_OBJECT(Engine, Object);
  void Start();

 public:
  explicit Engine(Context* context);
};

}  // namespace Voxels
