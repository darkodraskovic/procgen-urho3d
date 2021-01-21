#include <Urho3D/Engine/Application.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/Scene/Scene.h>

using namespace Urho3D;

class App : public Application
{
public:
    App(Context* context);
    virtual void Setup();
    virtual void Start();
    virtual void Stop();
    void HandleKeyDown(StringHash eventType, VariantMap& eventData);

    void MoveCamera(float timeStep);    
    void CreateScene();
    void SetupViewport();
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    void SubscribeToEvents();

private:
    SharedPtr<Scene> scene_;
    /// Camera scene node.
    SharedPtr<Node> cameraNode_;
    /// Camera yaw angle.
    float yaw_;
    /// Camera pitch angle.
    float pitch_;    
    
};
