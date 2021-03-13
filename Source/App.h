#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/DebugRenderer.h>

using namespace Urho3D;

class App : public Application
{
public:
    App(Context* context);
    virtual void Setup();
    virtual void Start();
    virtual void Stop();
    void HandleKeyDown(StringHash eventType, VariantMap& eventData);

    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    void SubscribeToEvents();

private:
};
