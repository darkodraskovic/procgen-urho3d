#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>

#include "Subsystems/SceneManager.h"
#include "Subsystems/ControllerManager.h"

#include "Components/CameraController.h"
#include "Components/CharacterController.h"

using namespace Urho3D;

namespace FPS {

    class Engine : public Object {
        URHO3D_OBJECT(Engine, Object);
        void Start();

    public:
        explicit Engine (Context* context);
    };

}
