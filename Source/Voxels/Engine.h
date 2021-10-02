#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>

#include "Subsystems/Utils.h"
#include "Subsystems/World.h"
#include "Components/Block.h"
#include "Components/Chunk.h"

using namespace Urho3D;

namespace Voxels {

    class Engine : public Object {
        URHO3D_OBJECT(Engine, Object);
        void Start();

    public:
        explicit Engine (Context* context);
    };

}
