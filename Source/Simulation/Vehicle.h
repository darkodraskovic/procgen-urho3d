#pragma once

#include <Urho3D/Urho3DAll.h>

namespace Simulation {

class Vehicle : public LogicComponent {
  URHO3D_OBJECT(Vehicle, LogicComponent);

 public:
  explicit Vehicle(Context* context);
  virtual void Start() override;
  virtual void Update(float timeStep) override;
  // virtual void OnSetEnabled() override;
  virtual void DelayedStart() override;

  void Steer(const Vector3& target);
  void Wander();
  Node* target_ = nullptr;

 private:
  bool arrived_ = false;

  float maxSpeed_ = 3.f;
  float maxForce_ = 3.f;
  float angularIntertia_ = .05f;
  float slowdownTreshlod_ = 1.f;

  float wanderRadius_ = 2.f;
  float wanderDistance_ = 4.f;
};

}  // namespace Simulation
