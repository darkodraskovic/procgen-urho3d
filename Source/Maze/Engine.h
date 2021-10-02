#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>

#include "Engine.h"
#include "Crawler.h"

using namespace Urho3D;

namespace Maze {

    class Engine : public Object {
        URHO3D_OBJECT(Engine, Object);
        void Start();

    public:
        explicit Engine (Context* context);
    };
}
