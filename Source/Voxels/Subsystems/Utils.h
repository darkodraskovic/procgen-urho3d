#pragma once

#include <Urho3D/Urho3DAll.h>

#include <libtcod/noise.hpp>

namespace Voxels {

class Utils : public Object {
  URHO3D_OBJECT(Utils, Object);

 public:
  explicit Utils(Context* context);
  float Map(float newMin, float newMax, float min, float max, float value);
  int GenerateHeight(float x, float z, int height = 128, float smooth = 0.01,
                     int octaves = 4);
  float GenerateProbability(float x, float y, float z, float smooth = 0.01,
                            int octaves = 3);

  TCODNoise* noise2D_;
  TCODNoise* noise3D_;

 private:
  TCODRandom* random_;
};

}  // namespace Voxels
