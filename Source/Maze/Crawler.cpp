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
    for (int i = 0; i < 3; ++i) {
        CrawlH();
    }
    for (int i = 0; i < 3; ++i) {
        CrawlV();        
    }
}

void Crawler::CrawlH(){
    bool done = false;
    int x = 1;
    int z = Random(1, (int)height_-1);

    while (!done) {
        map_[x][z] = CELL_EMPTY;

        if (Rand() % 3) x += Random(0, 2);
        else z += Random(-1, 2);

        done |= (x >= width_-1 || z == 1 || z >= height_-1);
    }
};

void Crawler::CrawlV(){
    bool done = false;
    int x = Random(1, (int)width_-1);
    int z = 1;

    while (!done) {
        map_[x][z] = CELL_EMPTY;

        if (Rand() % 3) z += Random(0, 2);
        else x += Random(-1, 2);

        done |= (x == 1 || x >= width_-1 || z >= height_-1);
    }
};
