#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Scene.h>

using namespace Urho3D;

namespace Toy {

    class ProcModelToy : public Object {
        URHO3D_OBJECT(ProcModelToy, Object);

    public:
        explicit ProcModelToy (Context* context);
        Node* CreateModel();

        void Start();
    };
}
