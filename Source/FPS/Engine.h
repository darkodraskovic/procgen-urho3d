#pragma once

#include <Urho3D/Urho3DAll.h>

#include "Components/CameraController.h"
#include "Components/CharacterController.h"
#include "Subsystems/ControllerManager.h"
#include "Subsystems/SceneManager.h"

namespace FPS {

class Engine : public Object {
  URHO3D_OBJECT(Engine, Object);
  void Start();

 public:
  explicit Engine(Context* context);
};

}  // namespace FPS
