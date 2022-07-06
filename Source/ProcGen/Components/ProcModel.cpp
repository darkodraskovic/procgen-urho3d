#include "ProcModel.h"

using namespace ProcGen;

ProcModel::ProcModel(Context* context) : LogicComponent(context) {
  SetUpdateEventMask(LogicComponentEvents::None);
}

void ProcModel::CalculateNormals() {
  // works only when len(indices) == len(vertices)
  normals_.Clear();
  for (int i = 0; i < indices_.Size(); i += 3) {
    Vector3 edge1 = positions_[indices_[i + 1]] - positions_[indices_[i]];
    Vector3 edge2 = positions_[indices_[i + 2]] - positions_[indices_[i]];
    Vector3 normal = edge1.CrossProduct(edge2).Normalized();
    normals_.Push(normal);
    normals_.Push(normal);
    normals_.Push(normal);
  }
}

void ProcModel::GenerateData() {
  if (!normals_.Size()) CalculateNormals();

  vertexElements_.Push({TYPE_VECTOR3, SEM_POSITION});
  if (normals_.Size()) vertexElements_.Push({TYPE_VECTOR3, SEM_NORMAL});
  if (colors_.Size()) vertexElements_.Push({TYPE_VECTOR4, SEM_COLOR});
  if (uvs_.Size()) vertexElements_.Push({TYPE_VECTOR2, SEM_TEXCOORD});

  vectorBuffer_.Clear();
  for (int i = 0; i < positions_.Size(); i++) {
    if (positions_.Size()) vectorBuffer_.WriteVector3(positions_[i]);
    if (normals_.Size()) vectorBuffer_.WriteVector3(normals_[i]);
    if (colors_.Size()) vectorBuffer_.WriteColor(colors_[i]);
    if (uvs_.Size()) vectorBuffer_.WriteVector2(uvs_[i]);
  }
}

void ProcModel::Commit() {
  // buffers
  SharedPtr<VertexBuffer> vertexBuffer(new VertexBuffer(context_));
  vertexBuffer->SetShadowed(true);
  vertexBuffer->SetSize(positions_.Size(), vertexElements_);
  vertexBuffer->SetData((void*)vectorBuffer_.GetData());

  SharedPtr<IndexBuffer> indexBuffer(new IndexBuffer(context_));
  indexBuffer->SetShadowed(true);
  indexBuffer->SetSize(indices_.Size(), false, true);
  indexBuffer->SetData(indices_.Buffer());

  // geometry
  Geometry* geometry = new Geometry(context_);
  geometry->SetNumVertexBuffers(1);
  geometry->SetVertexBuffer(0, vertexBuffer);
  geometry->SetIndexBuffer(indexBuffer);
  geometry->SetDrawRange(primitiveType_, 0, indexBuffer->GetIndexCount());

  // model
  Model* model = new Model(context_);
  model->SetNumGeometries(1);
  model->SetGeometry(0, 0, geometry);

  // TODO
  // Optional. Morph ranges could also be not defined. Define a zero range (no
  // morphing) for the vertex buffer.
  Vector<unsigned> morphRangeStarts;
  Vector<unsigned> morphRangeCounts;
  morphRangeStarts.Push(0);
  morphRangeCounts.Push(0);
  model->SetVertexBuffers({vertexBuffer}, morphRangeStarts, morphRangeCounts);
  model->SetIndexBuffers({indexBuffer});
  // Construct from an array of vertices.
  model->SetBoundingBox(BoundingBox(&positions_.Front(), positions_.Size()));

  auto* staticModel = node_->GetComponent<StaticModel>();
  if (!staticModel) staticModel = node_->CreateComponent<StaticModel>();
  staticModel->SetModel(model);
  if (material_.NotNull()) staticModel->SetMaterial(material_);
}

void ProcModel::SetDrawNormals(bool enabled) {
  if (enabled) {
    drawNormals_ = true;

    if (HasSubscribedToEvent(E_POSTRENDERUPDATE))
      return;
    else
      SubscribeToEvent(E_POSTRENDERUPDATE,
                       URHO3D_HANDLER(ProcModel, HandlePostRenderUpdate));

    return;
  }
  UnsubscribeFromEvent(E_POSTRENDERUPDATE);
  drawNormals_ = false;
}

bool ProcModel::GetDrawNormals() { return drawNormals_; }

void ProcModel::HandlePostRenderUpdate(StringHash eventType,
                                       VariantMap& eventData) {
  if (drawNormals_) {
    auto* debugRenderer = GetScene()->GetComponent<DebugRenderer>();
    auto tnf = node_->GetWorldTransform();
    auto rot = node_->GetWorldRotation().RotationMatrix();
    auto scene = GetScene();
    for (int i = 0; i < positions_.Size(); ++i) {
      auto pos = tnf * positions_[i];
      debugRenderer->AddLine(pos, pos + rot * normals_[i], Color::RED);
    }
  }
}
