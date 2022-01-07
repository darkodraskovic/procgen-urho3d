#pragma once

#include <Urho3D/Urho3DAll.h>

namespace ProcGen {

class GeometryCreator : public Object {
  URHO3D_OBJECT(GeometryCreator, Object);

 public:
  explicit GeometryCreator(Context* context);

  CustomGeometry* CreateCustomGeometry(PrimitiveType type, int numVertices,
                                       bool hasNormals = true,
                                       bool hasColors = true,
                                       bool hasTexCoords = false,
                                       bool hasTangents = false);

  void CreateTestGeometry();
};
}  // namespace ProcGen
