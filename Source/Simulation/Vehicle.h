#pragma once

#include <Urho3D/Core/Context.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

namespace Simulation {

    class Vehicle : public LogicComponent {
        URHO3D_OBJECT(Vehicle, LogicComponent);

    public:
        explicit Vehicle (Context* context);
        virtual void Start () override;
        virtual void Update (float timeStep) override;
        // virtual void OnSetEnabled() override;
        virtual void DelayedStart() override;
        
        void Steer(const Vector3& target);
        Node* target_ = nullptr;
        
    private:
        float maxSpeed_ = 4.;
        float maxForce_ = 4.;
        float angularIntertia_ = .05f;
    };

}
