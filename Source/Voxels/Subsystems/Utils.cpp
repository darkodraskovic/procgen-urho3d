#include <Urho3D/IO/Log.h>
#include <libtcod/noise.h>

#include "Utils.h"

using namespace Voxels;

Utils::Utils(Context* context) : Object(context) {
    random_ = new TCODRandom();
    noise2D_ = new TCODNoise(2,random_);
    noise2D_->setType(TCOD_NOISE_SIMPLEX);

    minHeight_ = 100;
    maxHeight_ = 150;
    smooth_ = 0.01f;
    octaves_ = 4;
    persistence_ = .5f;
}

float Utils::Map (float newMin, float newMax, float min, float max, float value) {
    return Lerp(newMin, newMax, InverseLerp(min, max, value));
}

int Utils::GenerateHeight(float x, float y) {
    float f[2] = {x*smooth_, y*smooth_};
    // fbm returns value between -1.0 and 1.0
    float height = Map(minHeight_, maxHeight_, -1, 1, noise2D_->getFbm(f, octaves_));
    return (int)(height);
}
