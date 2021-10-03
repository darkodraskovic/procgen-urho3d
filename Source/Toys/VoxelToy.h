#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>

#include "../FPS/Components/CharacterController.h"
#include "../FPS/Components/CameraController.h"

using namespace Urho3D;

namespace Toy {

    class VoxelToy : public Object {
        URHO3D_OBJECT(VoxelToy, Object);

    public:
        explicit VoxelToy (Context* context);
        void Start();
        void HandleKeyDown(StringHash eventType, VariantMap& eventData);
        
        void CreateVoxels();
        void SetupPlayer();

    private:
        WeakPtr<FPS::CameraController> camController_ = nullptr;
        WeakPtr<FPS::CharacterController> charController_ = nullptr;
        bool firstPerson_ = false;
    };
}
