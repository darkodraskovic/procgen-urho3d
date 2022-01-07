#pragma once

#include <Urho3D/Urho3DAll.h>

#include "Components/ProcModel.h"
#include "Subsystems/GeometryCreator.h"
#include "Subsystems/MaterialCreator.h"
#include "Subsystems/ModelCreator.h"
#include "Subsystems/TextureCreator.h"

namespace ProcGen {

class Engine : public Object {
  URHO3D_OBJECT(Engine, Object);
  void Start();

 public:
  explicit Engine(Context* context);
};

}  // namespace ProcGen
