#pragma once

#include <Urho3D/Container/Vector.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Scene/LogicComponent.h>
#include "Block.h"

using namespace Urho3D;

namespace Voxels {

    class Chunk : public LogicComponent {
        URHO3D_OBJECT(Chunk, LogicComponent);

    public:
        explicit Chunk (Context* context);
        virtual void Start () override;
        // virtual void Update (float timeStep) override;

        void Build();
        void Model();
        bool IsTransparent(BlockData* data, int x, int y, int z);

    private:
        Vector<Vector<Vector<BlockData>>> blocks_;
    };

}
