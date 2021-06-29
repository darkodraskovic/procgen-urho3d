#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Math/Random.h>
#include <Urho3D/Scene/Node.h>

#include "Generator.h"

using namespace Maze;

Generator::Generator() {}

void Generator::Start() {}

void Generator::SetNode(Node* node) {
    node_ = node;
    parent_ = node->GetParent();
    if (!parent_) URHO3D_LOGERROR("Generator::SetNode: node must have a parent!");
}

void Generator::InitializeMap(unsigned int width, unsigned int height) {
    map_ = Vector<Vector<unsigned>>(width, Vector<unsigned>(height, CELL_WALL));
    width_ = width;
    height_ = height;
}

void Generator::Generate() {
    for (int x = 0; x < width_; x++) {
        for (int z = 0; z < height_; z++) {
            if (Rand() % 2) map_[x][z] = CELL_WALL;
        }
    }
}

void Generator::Draw() {
    for (int x = 0; x < width_; x++) {
        for (int z = 0; z < height_; z++) {
            switch (map_[x][z]) {
            case CELL_EMPTY:
                break;
            case CELL_WALL:
                auto* node = node_->Clone();
                auto pos = node->GetPosition();
                pos.x_ = x; pos.z_ = z;
                node->SetPosition(pos);
                break;
            }
        }
    }
}

int Generator::CountSquareNeighbors(int x, int z) {
    int count = 0;

    if (x <= 0 || x >= width_ - 1 || z <= 0 || z >= height_ - 1) return 5;
    
    if (map_[x-1][z] == 0) count++;
    if (map_[x+1][z] == 0) count++;
    if (map_[x][z-1] == 0) count++;
    if (map_[x][z+1] == 0) count++;
    
    return count;
}

int Generator::CountDiagonalNeighbors(int x, int z) {
    int count = 0;

    if (x <= 0 || x >= width_ - 1 || z <= 0 || z >= height_ - 1) return 5;
    
    if (map_[x-1][z-1] == 0) count++;
    if (map_[x+1][z-1] == 0) count++;
    if (map_[x-1][z+1] == 0) count++;
    if (map_[x+1][z+1] == 0) count++;
    
    return count;
}

int Generator::CountAllNeighbors(int x, int z) {
    return CountSquareNeighbors(x, z) + CountDiagonalNeighbors(x, z);
}
