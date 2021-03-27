#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Scene/Node.h>

using namespace Urho3D;

namespace Voxels {

    class World : public Object {
        URHO3D_OBJECT(World, Object);

    public:
        explicit World (Context* context);

        void BuildColumn(int x, int z);
        void SetRoot(Node* node);
        void SetMaterial(Material* material);
        
        IntVector2 size_ = {4,4};
        int chunkSize_ = 16;
        int columnHeight_ = 16;

    private:
        HashMap<String, Node*> chunks_;
        SharedPtr<Node> root_;
        SharedPtr<Material> material_;
    };
}
