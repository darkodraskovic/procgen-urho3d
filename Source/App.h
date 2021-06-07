#include <Urho3D/Engine/Application.h>
#include <Urho3D/Input/Controls.h>
#include <Urho3D/Scene/Scene.h>

using namespace Urho3D;

class App : public Application
{
public:
    App(Context* context);
    virtual void Setup();
    virtual void Start();
    virtual void Stop();

private:
    void HandleKeyDown(StringHash eventType, VariantMap& eventData);
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    void HandlePostUpdate(StringHash eventType, VariantMap& eventData);

    void SubscribeToEvents();
    void CreateConsoleAndDebugHud();

    void CreateProceduralModel();

    SharedPtr<Scene> scene_;
};
