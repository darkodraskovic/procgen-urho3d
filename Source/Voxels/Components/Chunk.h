#pragma once

#include "Block.h"

namespace Voxels {

class Chunk : public LogicComponent {
  URHO3D_OBJECT(Chunk, LogicComponent);

 public:
  explicit Chunk(Context* context);
  virtual void Start() override;
  // virtual void Update (float timeStep) override;

  void Build();
  void Model();
  bool IsTransparent(BlockData* data, int x, int y, int z);

 private:
  Vector<Vector<Vector<BlockData>>> blocks_;
};

}  // namespace Voxels
