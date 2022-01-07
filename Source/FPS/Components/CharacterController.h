#pragma once

#include <Urho3D/Urho3DAll.h>

namespace FPS {

const float MOVE_FORCE = 1.2f;
const float INAIR_MOVE_FORCE = 0.02f;
const float BRAKE_FORCE = 0.2f;
const float JUMP_FORCE = 7.0f;
const float INAIR_THRESHOLD_TIME = 0.1f;

/// Character component, responsible for physical movement according to
/// controls, as well as animation.
class CharacterController : public LogicComponent {
  URHO3D_OBJECT(CharacterController, LogicComponent);

 public:
  /// Construct.
  explicit CharacterController(Context* context);

  /// Register object factory and attributes.
  static void RegisterObject(Context* context);

  void Start() override;
  // void OnSceneSet(Scene* scene) override;
  void Update(float timeStep) override;
  void FixedUpdate(float timeStep) override;
  void PostUpdate(float timeStep) override;

  // Referenced by ControllerManager
  Urho3D::Controls controls_;

 private:
  /// Handle physics collision event.
  void HandleNodeCollision(StringHash eventType, VariantMap& eventData);

  /// Grounded flag for movement.
  bool onGround_;
  /// Jump flag.
  bool okToJump_;
  /// In air timer. Due to possible physics inaccuracy, character can be off
  /// ground for max. 1/10 second and still be allowed to move.
  float inAirTimer_;
};
}  // namespace FPS
