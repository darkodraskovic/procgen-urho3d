#pragma once

#include <Urho3D/Urho3DAll.h>

namespace ProcGen {
class TextureCreator : public Object {
  URHO3D_OBJECT(TextureCreator, Object);

 public:
  explicit TextureCreator(Context* context);

  void Start();
  Texture2D* CreateImageTexture(Image* image);
  Texture2D* CreateRenderTexture(
      int w, int h, RenderSurfaceUpdateMode mode = SURFACE_UPDATEVISIBLE);
  Texture2D* CreateEffectTexture(
      int w, int h, const String& shader,
      RenderSurfaceUpdateMode mode = SURFACE_UPDATEVISIBLE);

  void SetScene(Scene* scene);

 private:
  void CreateCamera();

  SharedPtr<Scene> scene_;
  Node* cameraNode_;
};
}  // namespace ProcGen
