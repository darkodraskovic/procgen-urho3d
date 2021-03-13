#pragma once

#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

namespace ProcGen {

    class CameraController : public LogicComponent
    {
        URHO3D_OBJECT(CameraController, LogicComponent)
        
    public:
        explicit CameraController(Context* context);
        void Update(float timeStep) override;
        void DelayedStart() override;

    private:
        /// Camera yaw angle.
        float yaw_;
        /// Camera pitch angle.
        float pitch_;
    };
}
