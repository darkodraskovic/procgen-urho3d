#pragma once

#include "Vehicle.h"

namespace Simulation {

class Engine : public Object {
  URHO3D_OBJECT(Engine, Object);
  void Start();

 public:
  explicit Engine(Context* context);
};

}  // namespace Simulation
