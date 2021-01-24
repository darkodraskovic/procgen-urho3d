#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Math/BoundingBox.h>
#include <iostream>

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Math/Color.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/GraphicsDefs.h>

#include "ProcModel.h"

using namespace ProcGen;

ProcModel::ProcModel(Context* context) : LogicComponent(context) {
    SetUpdateEventMask(USE_NO_EVENT);
}

void ProcModel::Start() {
    node_->CreateComponent<StaticModel>();
}

void ProcModel::SetVertexBuffer(void* data, VertexMask semantics, unsigned numVertices) {
    if (!numVertices) numVertices = vertexBuffers_.At(0)->GetVertexCount();
    VertexBuffer* buffer = new VertexBuffer(context_);
    buffer->SetSize(numVertices, semantics);
    buffer->SetData(data);
    buffer->SetShadowed(true);
    vertexBuffers_.Push(SharedPtr<VertexBuffer>(buffer));
}

void ProcModel::SetIndices(unsigned short* data, unsigned numIndices){
    if (!numIndices) numIndices = vertexBuffers_.At(0)->GetVertexCount();
    IndexBuffer* indices = new IndexBuffer(context_);
    indices->SetSize(numIndices, false);
    indices->SetData(data);
    indices->SetShadowed(true);
    indexBuffers_.Push(SharedPtr<IndexBuffer>(indices));
}

void ProcModel::CalculateNormals() {
    normals_.Clear();
    for (int i = 0; i < indices_.Size(); i += 3) {
        Vector3 edge1 = positions_[indices_[i+1]] - positions_[indices_[i]];
        Vector3 edge2 = positions_[indices_[i+2]] - positions_[indices_[i]];
        Vector3 normal = edge1.CrossProduct(edge2).Normalized();
        normals_.Push(normal); normals_.Push(normal); normals_.Push(normal);
    }
}

void ProcModel::Generate() {
    model_ = new Model(context_);

    if (!normals_.Size()) CalculateNormals();
    
    SetVertexBuffer(positions_.Buffer(), MASK_POSITION, positions_.Size());
    if (normals_.Size()) SetVertexBuffer(normals_.Buffer(), MASK_NORMAL);
    if (colors_.Size()) SetVertexBuffer(colors_.Buffer(), MASK_COLOR);
    SetIndices(indices_.Buffer(), indices_.Size());

    Geometry* geometry = new Geometry(context_);
    model_->SetNumGeometries(1);
    model_->SetGeometry(0, 0, geometry);

    geometry->SetNumVertexBuffers(vertexBuffers_.Size());
    for (int i = 0; i < vertexBuffers_.Size(); i++) {
        geometry->SetVertexBuffer(i, vertexBuffers_.At(i));
    }
    geometry->SetIndexBuffer(indexBuffers_.At(0));
    geometry->SetDrawRange(primitiveType_, 0, indexBuffers_.At(0)->GetIndexCount());

    // TODO
    // Optional. Morph ranges could also be not defined. Define a zero range (no morphing) for the vertex buffer.
    PODVector<unsigned> morphRangeStarts;
    PODVector<unsigned> morphRangeCounts;
    morphRangeStarts.Push(0);
    morphRangeCounts.Push(0);
    model_->SetVertexBuffers(vertexBuffers_, morphRangeStarts, morphRangeCounts);
    model_->SetIndexBuffers(indexBuffers_);

    auto* staticModel = node_->GetComponent<StaticModel>();
    staticModel->SetModel(model_);
    if (material_.NotNull()) staticModel->SetMaterial(material_);
    
    // TODO
    // model->SetBoundingBox(BoundingBox(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f)));
    
    vertexBuffers_.Clear();
    indexBuffers_.Clear();
}

void ProcModel::SetDrawNormals(bool enabled) {
    if (enabled) {
        drawNormals_ = true;
        
        if (HasSubscribedToEvent(E_POSTRENDERUPDATE)) return;
        else SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(ProcModel, HandlePostRenderUpdate));

        return;
    }
    UnsubscribeFromEvent(E_POSTRENDERUPDATE);
    drawNormals_ = false;
}

void ProcModel::HandlePostRenderUpdate (StringHash eventType, VariantMap& eventData) {
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