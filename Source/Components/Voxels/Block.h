#pragma once

#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Math/Vector2.h>
#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

namespace ProcGen {
    enum BlockFace {BOTTOM, TOP, LEFT, RIGHT, FRONT, BACK};
    enum BlockType {DIRT, STONE, EARTH, GRASS_SIDE, GRASS_TOP, AIR};
    struct BlockData {
        BlockType type_;
        bool transparent_;
    };
    
    class Block : public LogicComponent {
        URHO3D_OBJECT(Block, LogicComponent);

    public:
        explicit Block (Context* context);

        void CreateQuad(BlockFace face, BlockType type, const Vector3& position = Vector3::ZERO);
        
        static Vector<Vector2> GetTextureCoords(int x, int y);
    };
}
