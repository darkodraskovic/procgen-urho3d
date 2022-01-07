#pragma once

#include "Crawler.h"
#include "Engine.h"

namespace Maze {

class Engine : public Object {
  URHO3D_OBJECT(Engine, Object);
  void Start();

 public:
  explicit Engine(Context* context);
};
}  // namespace Maze
