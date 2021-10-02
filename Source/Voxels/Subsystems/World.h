#pragma once

#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Core/WorkQueue.h>

using namespace Urho3D;

namespace Voxels {
    class World : public Object {
        URHO3D_OBJECT(World, Object);

    public:
        explicit World (Context* context);
        void Start();
        
        void CreateColumn(int x, int z);
        void CreateColumns();
        void Build();
        void Model();
        void HandlWorkItemCompleted(StringHash eventType, VariantMap& eventData);
        void SetRoot(Node* node);
        Node* GetRoot();
        void SetMaterial(Material* material);
        Material* GetMaterial(void);
        
        String GetChunkName(int x, int y, int z);
        Vector3 GetWorldSize();
        Vector3 GetWorldPosition(int x, int y, int z);
        IntVector3 WorldToBlock(const Vector3& position);

        WorkQueue* GetWorkQueue();

        static void BuildAsync(const WorkItem* workItem, unsigned threadIndex);
        
        IntVector3 size_ = {4,16,4};
        int chunkSize_ = 16;

    private:
        SharedPtr<Node> root_;
        SharedPtr<Material> material_;
        SharedPtr<WorkQueue> workQueue_;
        unsigned int workItemCounter_ = 0;
    };
}
