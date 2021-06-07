#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Input/Controls.h>

using namespace Urho3D;

namespace ProcGen {

    class Controller : public Object {
        URHO3D_OBJECT(Controller, Object);

    public:
        explicit Controller (Context* context);
        
        void Start();
        void HandleUpdate(StringHash eventType, VariantMap& eventData);

        void SetControls(Urho3D::Controls* controls);
        Urho3D::Controls* controls_;
    };

}
