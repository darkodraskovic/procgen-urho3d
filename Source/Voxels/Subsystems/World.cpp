#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>

#include "../../ProcGen/Subsystems/TextureCreator.h"
#include "../../ProcGen/Components/ProcModel.h"

#include "World.h"
#include "../Components/Block.h"
#include "../Components/Chunk.h"

using namespace Voxels;

World::World(Context* context) : Object(context) {}

void World::SetRoot(Node* node) {
    root_ = node;
}

void World::SetMaterial(Material* material) {
    material_ = material;
}

void World::BuildColumn(int x, int z) {
    for (int y = 0; y < columnHeight_; y++) {
        Vector3 position = {(float)x, (float)y, (float)z};
        position *= chunkSize_;
        
        Node* node = root_->CreateChild(position.ToString());
        node->SetWorldPosition(position);
        
        ProcGen::ProcModel* procModel = node->CreateComponent<ProcGen::ProcModel>();
        Voxels::Block* block = node->CreateComponent<Voxels::Block>();
        Voxels::Chunk* chunk = node->CreateComponent<Voxels::Chunk>();
        chunk->Build();

        procModel->material_ = material_;
        procModel->Generate();
        node->GetComponent<StaticModel>()->SetCastShadows(true);

        // auto* body = node->CreateComponent<RigidBody>();
        // body->SetMass(1);
        // body->SetUseGravity(false);
        // body->SetAngularVelocity(Vector3::UP*1.5);
    }
}
