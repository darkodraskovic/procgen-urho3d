#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/CustomGeometry.h>
#include <Urho3D/Scene/Scene.h>

using namespace Urho3D;

namespace Toy {

    class SimulationToy : public Object {
        URHO3D_OBJECT(SimulationToy, Object);

    public:
        explicit SimulationToy (Context* context);
        void Start();
        void HandleUpdate(StringHash eventType, VariantMap& eventData);

        Node* CreateVehicle();
        
    private:
        Scene* scene_;
    };
}
