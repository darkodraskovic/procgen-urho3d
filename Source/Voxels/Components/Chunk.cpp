#include <Urho3D/Container/Vector.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Math/BoundingBox.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Math/Random.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Scene/Node.h>

#include "../../ProcGen/Components/ProcModel.h"

#include "Block.h"
#include "Chunk.h"

using namespace ProcGen;
using namespace Voxels;

Chunk::Chunk(Context* context) : LogicComponent(context) {
    SetUpdateEventMask(USE_NO_EVENT);

    SetRandomSeed(GetSubsystem<Time>()->GetSystemTime());
    size_ = {16, 16, 16};
    for(int x = 0; x < size_.x_; x++) {
        blocks_.Push(Vector<Vector<BlockData>>{});
        for(int y = 0; y < size_.y_; y++) {
            blocks_[x].Push(Vector<BlockData>{});
            for(int z = 0; z < size_.z_; z++) {
                if (Random() > .3) blocks_[x][y].Push({DIRT, false});
                else blocks_[x][y].Push({AIR, true});
            }
        }
    }
}

// void Chunk::Start() {}
// void Chunk::Update(float timeStep) {}

void Chunk::Build() {
    auto* block = node_->GetComponent<Voxels::Block>();

    for(int x = 0; x < size_.x_; x++) {
        for(int y = 0; y < size_.y_; y++) {
            for(int z = 0; z < size_.z_; z++) {
                auto& blockData = blocks_[x][y][z];
                
                if (blockData.type_ == AIR) continue;;
                
                Vector3 pos = {(float)x, (float)y, (float)z};
                if (IsTransparent(x-1, y, z)) {
                    block->CreateQuad(LEFT, blockData.type_, pos);
                }
                if (IsTransparent(x+1, y, z)) {
                    block->CreateQuad(RIGHT, blockData.type_, pos);
                }
                if (IsTransparent(x, y-1, z)) {
                    block->CreateQuad(BOTTOM, blockData.type_, pos);
                }
                if (IsTransparent(x, y+1, z)) {
                    block->CreateQuad(TOP, blockData.type_, pos);
                }
                if (IsTransparent(x, y, z-1)) {
                    block->CreateQuad(BACK, blockData.type_, pos);
                }
                if (IsTransparent(x, y, z+1)) {
                    block->CreateQuad(FRONT, blockData.type_, pos);
                }
            }
        }
    }
}

bool Chunk::IsTransparent(int x, int y, int z) {
    if (x < 0 || x >= size_.x_ || y < 0 || y >= size_.y_ || z < 0 || z >= size_.z_) return true;
    return blocks_[x][y][z].transparent_;
}
