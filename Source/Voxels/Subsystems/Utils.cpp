#include <Urho3D/IO/Log.h>
#include <libtcod/noise.h>

#include "Utils.h"

using namespace Voxels;

Utils::Utils(Context* context) : Object(context) {
    random_ = new TCODRandom();
    noise2D_ = new TCODNoise(2,random_);
    noise2D_->setType(TCOD_NOISE_SIMPLEX);
}

float Utils::Map (float newMin, float newMax, float min, float max, float value) {
    return Lerp(newMin, newMax, InverseLerp(min, max, value));
}

int Utils::GenerateHeight(float x, float z, int height, float smooth, int octaves) {
    float f[2] = {x*smooth, z*smooth};
    // fbm returns value between -1.0 and 1.0
    return (int)(Map(0, height, -1, 1, noise2D_->getFbm(f, octaves)));
}
