#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Input/Controls.h>

using namespace Urho3D;

namespace FPS {

    // Urho3D::Controls::buttons_ is bitmask
    enum Controls{
        CTRL_FORWARD = 1,
        CTRL_BACK = 2,
        CTRL_LEFT = 4,
        CTRL_RIGHT = 8,
        CTRL_UP = 16,
        CTRL_DOWN = 32,
        CTRL_JUMP = 64,
    };
    
    class ControllerManager : public Object {
        URHO3D_OBJECT(ControllerManager, Object);

    public:
        explicit ControllerManager (Context* context);
        
        void Start();
        void HandleUpdate(StringHash eventType, VariantMap& eventData);

        void SetControls(Urho3D::Controls* controls);
        
        private:
        Urho3D::Controls* controls_;
    };

}
