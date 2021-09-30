#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>

#include "Subsystems/GeometryCreator.h"
#include "Subsystems/SceneManager.h"
#include "Subsystems/ModelCreator.h"
#include "Subsystems/TextureCreator.h"
#include "Subsystems/MaterialCreator.h"
#include "Subsystems/Controller.h"

#include "Components/CameraController.h"
#include "Components/ProcModel.h"

using namespace Urho3D;

namespace ProcGen {

    class Engine : public Object {
        URHO3D_OBJECT(Engine, Object);
        void Start();

    public:
        explicit Engine (Context* context);
    };

}
