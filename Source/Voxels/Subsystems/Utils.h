#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <libtcod/noise.hpp>

using namespace Urho3D;

namespace Voxels {

    class Utils : public Object {
        URHO3D_OBJECT(Utils, Object);

    public:
        explicit Utils (Context* context);
        float Map (float newMin, float newMax, float min, float max, float value);
        int GenerateHeight(float x, float z, int height = 128, float smooth = 0.01, int octaves = 4);

    private:
        TCODRandom* random_;
        TCODNoise* noise2D_;
    };

}
