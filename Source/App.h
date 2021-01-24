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
    void HandlePostRenderUpdate(StringHash /*eventType*/, VariantMap& eventData);

    void CreateScene();
    void SetupViewport();
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    void SubscribeToEvents();

private:
    SharedPtr<DebugRenderer> debugRenderer_;
    SharedPtr<Scene> scene_;
    /// Camera scene node.
    SharedPtr<Node> cameraNode_;
    /// Camera yaw angle.
    float yaw_;
    /// Camera pitch angle.
    float pitch_;    
    
};
