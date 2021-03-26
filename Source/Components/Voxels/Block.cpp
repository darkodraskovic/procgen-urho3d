#include <Urho3D/IO/Log.h>
#include <Urho3D/Math/Vector2.h>

#include "Block.h"
#include "../ProcModel.h"

using namespace ProcGen;


Vector<Vector3> points = {
    // clockwise winding, starts with (transformed) left bottom back
    // face idx: (0, 1, 2, 0, 2, 3) + (n = 4 * quad index)
    {-.5, -.5, -.5}, {-.5, .5, -.5}, {.5, .5, -.5}, {.5, -.5, -.5}, // back
    {-.5, -.5, .5}, {.5, -.5, .5}, {.5, .5, .5}, {-.5, .5, .5}, // front
};

// # cols/rows in the texture atlas
float size = 1. / 16;

// The order of UVs must match the order in enum BlockType
Vector<Vector <Vector2>> UVs = {
    Block::GetTextureCoords(0, 0), // DIRT
    Block::GetTextureCoords(1, 0), // STONE
    Block::GetTextureCoords(2, 0), // EARTH
    Block::GetTextureCoords(3, 0), // GRASS_SIDE
    Block::GetTextureCoords(2, 9), // GRASS_TOP
};

Block::Block(Context* context) : LogicComponent(context) {
    SetUpdateEventMask(USE_NO_EVENT);
}

void Block::CreateQuad(BlockFace face, BlockType type, const Vector3& position) {
    ProcModel* procModel = node_->GetComponent<ProcModel>();

    unsigned short idx = procModel->indices_.Size() ? procModel->indices_.Back() + 1 : 0;
    procModel->indices_.Push({idx, (unsigned short)(idx + 1), (unsigned short)(idx+ 2),
                              idx, (unsigned short)(idx+2), (unsigned short)(idx+3)});
    
    procModel->uvs_.Push({UVs[type][0], UVs[type][1], UVs[type][2], UVs[type][3]});
    
    switch(face) {
    case BACK:
        procModel->positions_.Push({points[0], points[1], points[2], points[3]});
        procModel->normals_.Push({Vector3::BACK, Vector3::BACK, Vector3::BACK, Vector3::BACK});
        break;
    case LEFT:
        procModel->positions_.Push({points[4], points[7], points[1], points[0]});
        procModel->normals_.Push({Vector3::LEFT, Vector3::LEFT, Vector3::LEFT, Vector3::LEFT});
        break;
    case FRONT:
        procModel->positions_.Push({points[5], points[6], points[7], points[4]});
        procModel->normals_.Push({Vector3::FORWARD, Vector3::FORWARD, Vector3::FORWARD, Vector3::FORWARD});
        break;
    case RIGHT:
        procModel->positions_.Push({points[3], points[2], points[6], points[5]});
        procModel->normals_.Push({Vector3::RIGHT, Vector3::RIGHT, Vector3::RIGHT, Vector3::RIGHT});
        break;
    case BOTTOM:
        procModel->positions_.Push({points[4], points[0], points[3], points[5]});
        procModel->normals_.Push({Vector3::DOWN, Vector3::DOWN, Vector3::DOWN, Vector3::DOWN});
        break;
    case TOP:
        procModel->positions_.Push({points[1], points[7], points[6], points[2]});
        procModel->normals_.Push({Vector3::UP, Vector3::UP, Vector3::UP, Vector3::UP});
        break;
    }

    int size = procModel->positions_.Size();
    for (int i = size-4; i < size; i++) {
        procModel->positions_[i] += position;
    }
}

Vector<Vector2> Block::GetTextureCoords(int x, int y) {
    // tex coords positive y points downwards
    return {{x * size, y * size + size}, {x * size, y * size},
            {x * size + size, y * size}, {x * size + size, y * size + size}};
}
