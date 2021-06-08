#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>

using namespace Urho3D;

namespace Toy {

    class ShaderToy : public Object {
        URHO3D_OBJECT(ShaderToy, Object);

    public:
        explicit ShaderToy (Context* context);

        void Start();
    };
}
