#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/UI/Slider.h>

#include "Voxels/Components/Character.h"

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
    // void HandlePostrenderupdate(StringHash eventType, VariantMap& eventData);

    // void HandleSlider0(StringHash eventType, VariantMap& eventData);
    // void HandleSlider1(StringHash eventType, VariantMap& eventData);
    // Slider* CreateSlider(int x, int y, int xSize, int ySize, const String& text);
    
    void SubscribeToEvents();
    void CreateConsoleAndDebugHud();

    void CreateStockModel();
    void CreateProceduralModel();
    void CreateVoxels();

    void CreateCharacter();
    
    SharedPtr<Scene> scene_;

    WeakPtr<Voxels::Character> player_;
    bool firstPerson_ = false;
};
