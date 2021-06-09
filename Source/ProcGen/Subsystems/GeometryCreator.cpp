#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Graphics/CustomGeometry.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Resource/ResourceCache.h>

#include "SceneManager.h"
#include "GeometryCreator.h"

using namespace ProcGen;

GeometryCreator::GeometryCreator(Context* context) : Object(context) {}

CustomGeometry* GeometryCreator::CreateCustomGeometry(PrimitiveType type, int numVertices, bool hasNormals, bool hasColors, bool hasTexCoords, bool hasTangents) {
    auto* cache = GetSubsystem<ResourceCache>();

    Node *customGeomNode = GetSubsystem<ProcGen::SceneManager>()->GetScene()->CreateChild();
    CustomGeometry* customGeom = customGeomNode->CreateComponent<CustomGeometry>();
    customGeom->SetNumGeometries(1);
    customGeom->SetMaterial(cache->GetResource<Material>("Materials/VColUnlit.xml"));
    customGeom->DefineGeometry(0, type, numVertices, hasNormals, hasColors, hasTexCoords, hasTangents);
    customGeom->SetDynamic(true);

    return customGeom;
}

void GeometryCreator::CreateTestGeometry() {
    CustomGeometry *customGeom = CreateCustomGeometry(PrimitiveType::TRIANGLE_LIST, 3);

    customGeom->DefineVertex(Vector3(0, 1, 0));
    customGeom->DefineNormal(Vector3(0, 0, -1));
    customGeom->DefineColor(Color::BLUE);
    customGeom->DefineVertex(Vector3(1, 0, 0));
    customGeom->DefineNormal(Vector3(0, 0, -1));
    customGeom->DefineColor(Color::RED);
    customGeom->DefineVertex(Vector3(0, 0, 0));
    customGeom->DefineNormal(Vector3(0, 0, -1));
    customGeom->DefineColor(Color::YELLOW);

    customGeom->Commit();
}

