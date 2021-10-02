#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Core/WorkQueue.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/UI/ProgressBar.h>
#include <Urho3D/UI/UI.h>

#include "../../ProcGen/Subsystems/TextureCreator.h"
#include "../../ProcGen/Components/ProcModel.h"

#include "World.h"
#include "../Components/Block.h"
#include "../Components/Chunk.h"

using namespace Voxels;

World::World(Context* context) : Object(context) {

}

void World::Start() {
  workQueue_ = new WorkQueue(context_);
  workQueue_->CreateThreads(7);

  SubscribeToEvent(E_WORKITEMCOMPLETED, URHO3D_HANDLER(World, HandlWorkItemCompleted));
}

WorkQueue *World::GetWorkQueue() { return workQueue_; }

void World::SetRoot(Node* node) { root_ = node; }

Node *World::GetRoot() { return root_; }

void World::SetMaterial(Material* material) {
    material_ = material;
}

Material *World::GetMaterial(void) {
    return material_;
}

void World::CreateColumn(int x, int z) {
    for (int y = 0; y < size_.y_; y++) {
        Vector3 position = Vector3(x, y, z) * chunkSize_;
        Node* node = root_->CreateChild(position.ToString());
        node->SetWorldPosition(position);

        ProcGen::ProcModel* procModel = node->CreateComponent<ProcGen::ProcModel>();
        procModel->material_ = material_;
        Voxels::Block* block = node->CreateComponent<Voxels::Block>();
        Voxels::Chunk* chunk = node->CreateComponent<Voxels::Chunk>();
        node->CreateComponent<RigidBody>();
        node->CreateComponent<CollisionShape>();
    }
}

void World::CreateColumns() {
    for (int x = 0; x < size_.x_; x++) {
        for (int z = 0; z < size_.z_; z++) {
            CreateColumn(x, z);
        }
    }
}

void World::Build() {
    for (int x = 0; x < size_.x_; x++) {
        for (int y = 0; y < size_.y_; y++) {
            for (int z = 0; z < size_.z_; z++) {
                Node* node = root_->GetChild(GetChunkName(x, y, z));
                SharedPtr<WorkItem> workItem(new WorkItem);
                workItem->workFunction_ = BuildAsync;
                workItem->aux_ = node->GetComponent<Chunk>();
                workItem->sendEvent_ = true;
                workQueue_->AddWorkItem(workItem);
            }
        }
    }
}

void World::BuildAsync(const WorkItem* workItem, unsigned threadIndex) {
    Chunk* chunk = (Chunk*)workItem->aux_;
    chunk->Build();
}

void World::Model() {
    for (int x = 0; x < size_.x_; x++) {
        for (int y = 0; y < size_.y_; y++) {
            for (int z = 0; z < size_.z_; z++) {
                Node* node = root_->GetChild(GetChunkName(x, y, z));
                node->GetComponent<Chunk>()->Model();
            }
        }
    }
}

void World::HandlWorkItemCompleted(StringHash eventType, VariantMap& eventData) {
    workItemCounter_++;
    
    int total = size_.x_ * size_.y_ * size_.z_;
    auto* ui = GetSubsystem<UI>();
    ProgressBar* bar = (ProgressBar*)ui->GetRoot()->GetChild("ProgressBar", true);
    float percent = (float)workItemCounter_ / total;
    bar->SetValue(percent * bar->GetRange());
    
    if (workItemCounter_ == total) {
        URHO3D_LOGINFO("DONE build"); // debug
        Model();
        URHO3D_LOGINFO("DONE model"); // debug
        workItemCounter_ = 0;
    }
}

Vector3 World::GetWorldSize() {
    return Vector3(size_ * chunkSize_);
}

Vector3 World::GetWorldPosition(int x, int y, int z) {
    return Vector3(x, y, z) * chunkSize_;
}

String World::GetChunkName(int x, int y, int z) {
    return GetWorldPosition(x, y, z).ToString();
}

IntVector3 World::WorldToBlock(const Vector3& position) {
    return IntVector3(position.x_, position.y_, position.z_) / chunkSize_;
}

void World::SetPlayer(FPS::CharacterController* player) {
    player_ = player;
}
