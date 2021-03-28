#include <Urho3D/IO/Log.h>
#include <Urho3D/Math/Random.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Scene/Node.h>

#include "../../ProcGen/Components/ProcModel.h"

#include "../Subsystems/World.h"
#include "../Subsystems/Utils.h"
#include "Block.h"
#include "Chunk.h"

using namespace ProcGen;
using namespace Voxels;

Chunk::Chunk(Context* context) : LogicComponent(context) {
    SetUpdateEventMask(USE_NO_EVENT);

}

void Chunk::Start() {
    auto* utils = GetSubsystem<Voxels::Utils>();
    Vector3 chunkPos = node_->GetPosition();
    auto* world = GetSubsystem<Voxels::World>();
    for(int x = 0; x < world->chunkSize_; x++) {
        blocks_.Push(Vector<Vector<BlockData>>{});
        for(int y = 0; y < world->chunkSize_; y++) {
            blocks_[x].Push(Vector<BlockData>{});
            for(int z = 0; z < world->chunkSize_; z++) {
                Vector3 pos = Vector3(x, y, z);
                Vector3 worldPos(chunkPos + pos);
                if (worldPos.y_ < utils->GenerateHeight(worldPos.x_, worldPos.z_)) {
                    blocks_[x][y].Push({pos, DIRT, false});
                }
                else {
                    blocks_[x][y].Push({pos, AIR, true});
                }
            }
        }
    }
}

// void Chunk::Update(float timeStep) {}

void Chunk::Build() {
    auto* block = node_->GetComponent<Voxels::Block>();

    auto* world = GetSubsystem<Voxels::World>();
    for(int x = 0; x < world->chunkSize_; x++) {
        for(int y = 0; y < world->chunkSize_; y++) {
            for(int z = 0; z < world->chunkSize_; z++) {
                auto& blockData = blocks_[x][y][z];
                
                if (blockData.type_ == AIR) continue;;
                
                Vector3 pos = {(float)x, (float)y, (float)z};
                if (IsTransparent(&blockData, x-1, y, z)) {
                    block->CreateQuad(LEFT, blockData.type_, pos);
                }
                if (IsTransparent(&blockData, x+1, y, z)) {
                    block->CreateQuad(RIGHT, blockData.type_, pos);
                }
                if (IsTransparent(&blockData, x, y-1, z)) {
                    block->CreateQuad(BOTTOM, blockData.type_, pos);
                }
                if (IsTransparent(&blockData, x, y+1, z)) {
                    block->CreateQuad(TOP, blockData.type_, pos);
                }
                if (IsTransparent(&blockData, x, y, z-1)) {
                    block->CreateQuad(BACK, blockData.type_, pos);
                }
                if (IsTransparent(&blockData, x, y, z+1)) {
                    block->CreateQuad(FRONT, blockData.type_, pos);
                }
            }
        }
    }
}

bool Chunk::IsTransparent(BlockData* data, int x, int y, int z) {
    auto* world = GetSubsystem<Voxels::World>();
    int chunkSize = world->chunkSize_;
    
    // if block in a neighbour chunk
    if (x < 0 || x >= chunkSize || y < 0 || y >= chunkSize || z < 0 || z >= chunkSize) {
        Vector3 neighbourPos = node_->GetPosition() +
            (Vector3(x, y, z) - data->position_) * chunkSize;
        
        auto* neighbour = world->GetRoot()->GetChild(neighbourPos.ToString());
        if (neighbour) {
            Chunk* chunk = neighbour->GetComponent<Chunk>();
            x = (x + chunkSize) % chunkSize;
            y = (y + chunkSize) % chunkSize;
            z = (z + chunkSize) % chunkSize;
            return chunk->blocks_[x][y][z].transparent_;
        }
        
        return true;
    };
    
    return blocks_[x][y][z].transparent_;
}

