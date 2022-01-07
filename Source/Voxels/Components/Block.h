#pragma once

#include <Urho3D/Urho3DAll.h>

namespace Voxels {
enum BlockFace { BOTTOM, TOP, LEFT, RIGHT, FRONT, BACK };
enum BlockType {
  GRAVEL,
  STONE,
  DIRT,
  BRICKS,
  GRASS_SIDE,
  GRASS_TOP,
  BEDROCK,
  DIAMOND,
  AIR
};
struct BlockData {
  Vector3 position_;  // chunk position
  BlockType type_;
  BlockType side_;
  BlockType bottom_;
  bool transparent_;
};

class Block : public LogicComponent {
  URHO3D_OBJECT(Block, LogicComponent);

 public:
  explicit Block(Context* context);

  void CreateQuad(BlockFace face, BlockType type,
                  const Vector3& position = Vector3::ZERO);

  static Vector<Vector2> GetTextureCoords(int x, int y);
};
}  // namespace Voxels
