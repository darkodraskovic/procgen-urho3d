#pragma once

#include <Urho3D/Urho3DAll.h>

namespace Toy {

class SimulationToy : public Object {
  URHO3D_OBJECT(SimulationToy, Object);

 public:
  explicit SimulationToy(Context* context);
  void Start();
  void HandleUpdate(StringHash eventType, VariantMap& eventData);

  Node* CreateVehicle();

 private:
  Scene* scene_;
};
}  // namespace Toy
