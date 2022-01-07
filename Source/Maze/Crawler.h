#pragma once

#include <Urho3D/Urho3DAll.h>

#include "Generator.h"

namespace Maze {

class Crawler : public Object, public Generator {
  URHO3D_OBJECT(Crawler, Object);
  void Start();
  void CrawlH();
  void CrawlV();

 public:
  explicit Crawler(Context* context);
  virtual void Generate() override;
};

}  // namespace Maze
