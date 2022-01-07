#pragma once

#include <Urho3D/Urho3DAll.h>

class App : public Application {
  URHO3D_OBJECT(App, Application);

 public:
  explicit App(Context* context);
  virtual void Setup() override;
  virtual void Start() override;
  virtual void Stop() override;

 private:
  void HandleKeyDown(StringHash eventType, VariantMap& eventData);
  void HandleUpdate(StringHash eventType, VariantMap& eventData);
  void HandlePostUpdate(StringHash eventType, VariantMap& eventData);

  void SubscribeToEvents();
  void CreateConsoleAndDebugHud();
};
