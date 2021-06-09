#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/CustomGeometry.h>

using namespace Urho3D;

namespace Toy {

    class GeometryToy : public Object {
        URHO3D_OBJECT(GeometryToy, Object);

    public:
        explicit GeometryToy (Context* context);
        void Start();
        void HandleUpdate(StringHash eventType, VariantMap& eventData);
        
        Node* CreateVehicle();
    };
}
