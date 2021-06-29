#pragma once

#include "Generator.h"
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>

using namespace Urho3D;

namespace Maze {

    class Crawler : public Object, public Generator {
        URHO3D_OBJECT(Crawler, Object);
        void Start();
        void CrawlH();
        void CrawlV();

    public:
        explicit Crawler (Context* context);
        virtual void Generate() override;
    };

}
