#include <Urho3D/Core/Timer.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Math/Random.h>

#include "Crawler.h"
#include "Generator.h"

using namespace Maze;

Crawler::Crawler(Context* context) : Object(context) {}

void Crawler::Start() {
    SetRandomSeed(GetSubsystem<Time>()->GetSystemTime());
}

void Crawler::Generate() {
    bool done = false;
    int x = width_ / 2;
    int z = height_ / 2;

    while (!done) {
        map_[x][z] = CELL_EMPTY;

        if (Rand() % 2) x += Random(-1, 2);
        else z += Random(-1, 2);
        
        done |= (x == 0 || x >= width_ || z == 0 || z >= height_);
    }
}
