#include <Urho3D/Graphics/Material.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Node.h>

#include "../../ProcGen/Subsystems/TextureCreator.h"
#include "../../ProcGen/Components/ProcModel.h"

#include "World.h"
#include "../Components/Block.h"
#include "../Components/Chunk.h"

using namespace Voxels;

World::World(Context* context) : Object(context) {}

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
                node->GetComponent<Chunk>()->Build();
            }
        }
    }
}

Vector3 World::GetSize() {
    return Vector3(size_ * chunkSize_);
}

Vector3 World::GetPosition(int x, int y, int z) {
    return Vector3(x, y, z) * chunkSize_;
}

String World::GetChunkName(int x, int y, int z) {
    return GetPosition(x, y, z).ToString();
}
