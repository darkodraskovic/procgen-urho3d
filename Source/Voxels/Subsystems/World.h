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

        void CreateColumn(int x, int z);
        void CreateColumns();
        void Build();
        void SetRoot(Node* node);
        Node* GetRoot();
        void SetMaterial(Material* material);
        Material* GetMaterial(void);
        
        String GetChunkName(int x, int y, int z);
        Vector3 GetSize();
        Vector3 GetPosition(int x, int y, int z);
        
        IntVector3 size_ = {4,16,4};
        int chunkSize_ = 16;

    private:
        HashMap<String, Node*> chunks_;
        SharedPtr<Node> root_;
        SharedPtr<Material> material_;
    };
}
