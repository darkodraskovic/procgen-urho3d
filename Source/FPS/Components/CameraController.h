#pragma once

#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Input/Controls.h>

using namespace Urho3D;

namespace FPS {
    
    class CameraController : public LogicComponent
    {
        URHO3D_OBJECT(CameraController, LogicComponent)

    public:
        explicit CameraController(Context* context);
        void Update(float timeStep) override;
        void Sync();

        // Referenced by ControllerManager
        Urho3D::Controls controls_;
        float moveSpeed_ = 12.0;

    private:
        float yaw_;
        float pitch_;
    };
}
