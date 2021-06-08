#pragma once

#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Input/Controls.h>

using namespace Urho3D;

namespace ProcGen {

    enum Controls{
        CTRL_FORWARD = 1,
        CTRL_BACK = 2,
        CTRL_LEFT = 4,
        CTRL_RIGHT = 8,
        CTRL_UP = 16,
        CTRL_DOWN = 32,
        CTRL_JUMP = 64,
    };
    
    class CameraController : public LogicComponent
    {
        URHO3D_OBJECT(CameraController, LogicComponent)
        
    public:
        explicit CameraController(Context* context);
        void Update(float timeStep) override;
        void UpdateRotation();
        
        Urho3D::Controls controls_;
        
    private:
        /// Camera yaw angle.
        float yaw_;
        /// Camera pitch angle.
        float pitch_;
    };
}
