#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/CustomGeometry.h>

using namespace Urho3D;

namespace ProcGen {

    class GeometryCreator : public Object {
        URHO3D_OBJECT(GeometryCreator, Object);

    public:
        explicit GeometryCreator (Context* context);

        CustomGeometry* CreateCustomGeometry(PrimitiveType type, int numVertices, bool hasNormals=true, bool hasColors=true, bool hasTexCoords=false, bool hasTangents=false);
        
        void CreateTestGeometry();
    };
}
