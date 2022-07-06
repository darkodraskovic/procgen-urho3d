#pragma once

#include <Urho3D/Urho3DAll.h>

namespace ProcGen {

class ProcModel : public LogicComponent {
  URHO3D_OBJECT(ProcModel, LogicComponent)
 public:
  explicit ProcModel(Context* context);

  // void Update(float timeStep) override;
  // virtual void PostUpdate (float timeStep) override;
  void HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData);
  void CalculateNormals();
  void GenerateData();
  void Commit();
  void SetDrawNormals(bool enabled);
  bool GetDrawNormals();

  Vector<Vector3> positions_;
  Vector<Vector3> normals_;
  Vector<Vector4> tangents_;
  // Vector<unsigned char> colors_;
  Vector<Color> colors_;
  Vector<Vector2> uvs_;
  Vector<unsigned short> indices_;
  SharedPtr<Material> material_;

  PrimitiveType primitiveType_ = TRIANGLE_LIST;

 private:
  bool drawNormals_ = false;

  VectorBuffer vectorBuffer_;
  Vector<VertexElement> vertexElements_;
};

}  // namespace ProcGen
