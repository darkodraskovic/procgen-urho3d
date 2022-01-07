#pragma once

#include <Urho3D/Urho3DAll.h>
namespace FPS {

class SceneManager : public Object {
  URHO3D_OBJECT(SceneManager, Object);

 public:
  explicit SceneManager(Context* context);
  void Start();

  Scene* GetScene();

  void SetupLights();
  void CreateSkybox(const String& material);
  void SetupEffects();

  Node* CreatePlayer();
  void SetFPS(bool enabled);

 private:
  void CreateScene();
  void SetupViewport();
  void SetupCamController();

  SharedPtr<DebugRenderer> debugRenderer_;
  SharedPtr<Scene> scene_;
};
}  // namespace FPS
