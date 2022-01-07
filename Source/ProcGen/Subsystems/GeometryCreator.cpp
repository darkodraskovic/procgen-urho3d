#include "GeometryCreator.h"

using namespace ProcGen;

GeometryCreator::GeometryCreator(Context* context) : Object(context) {}

CustomGeometry* GeometryCreator::CreateCustomGeometry(
    PrimitiveType type, int numVertices, bool hasNormals, bool hasColors,
    bool hasTexCoords, bool hasTangents) {
  auto* cache = GetSubsystem<ResourceCache>();

  Node* node = new Node(context_);
  CustomGeometry* customGeom = node->CreateComponent<CustomGeometry>();
  customGeom->SetNumGeometries(1);
  customGeom->SetMaterial(
      cache->GetResource<Material>("Materials/VColUnlit.xml"));
  customGeom->DefineGeometry(0, type, numVertices, hasNormals, hasColors,
                             hasTexCoords, hasTangents);
  customGeom->SetDynamic(true);

  return customGeom;
}

void GeometryCreator::CreateTestGeometry() {
  CustomGeometry* customGeom =
      CreateCustomGeometry(PrimitiveType::TRIANGLE_LIST, 3);

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
