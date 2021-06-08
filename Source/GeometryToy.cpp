#include <Urho3D/Graphics/CustomGeometry.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Resource/ResourceCache.h>

#include "ProcGen/Subsystems/SceneManager.h"

#include "GeometryToy.h"

using namespace Toy;

GeometryToy::GeometryToy(Context* context) : Object(context) {}

void GeometryToy::Start() {
    // URHO3D_LOGINFO("Geom Toy START");

    auto* cache = GetSubsystem<ResourceCache>();
    Scene* scene = GetSubsystem<ProcGen::SceneManager>()->GetScene();
    scene->GetComponent<Skybox>(true)->SetEnabled(false);

    auto* camNode = scene->GetChild("Camera");
    camNode->Translate(Vector3::BACK * 3);

    CreateTestGeometry();
}

CustomGeometry* GeometryToy::CreateCustomGeometry(PrimitiveType type, bool hasNormals, bool hasColors, bool hasTexCoords, bool hasTangents) {
    auto* cache = GetSubsystem<ResourceCache>();

    Node *customGeomNode = GetSubsystem<ProcGen::SceneManager>()->GetScene()->CreateChild();
    CustomGeometry* customGeom = customGeomNode->CreateComponent<CustomGeometry>();
    customGeom->SetNumGeometries(1);
    customGeom->SetMaterial(cache->GetResource<Material>("Materials/VColUnlit.xml"));
    customGeom->DefineGeometry(0, type, 3, true, true, false, false);
    customGeom->SetDynamic(true);

    return customGeom;
}

void GeometryToy::CreateTestGeometry() {
    CustomGeometry *customGeom = CreateCustomGeometry(PrimitiveType::TRIANGLE_LIST);

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
