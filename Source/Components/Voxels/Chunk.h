#pragma once

#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

namespace ProcGen {

    class Chunk : public LogicComponent {
        URHO3D_OBJECT(Chunk, LogicComponent);

    public:
        explicit Chunk (Context* context);
        virtual void Start () override;
        virtual void Update (float timeStep) override;
    };

}
