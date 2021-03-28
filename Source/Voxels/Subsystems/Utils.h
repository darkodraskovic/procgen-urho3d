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
        int GenerateHeight(float x, float y);
        float Map (float newMin, float newMax, float min, float max, float value);

        int minHeight_;
        int maxHeight_;
        float smooth_;
        int octaves_;
        float persistence_;
        
    private:
        TCODRandom* random_;
        TCODNoise* noise2D_;
    };

}
