#include <Urho3D/Container/Vector.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/GraphicsDefs.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/IO/VectorBuffer.h>
#include <Urho3D/Math/BoundingBox.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Math/Color.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Graphics/Geometry.h>

#include "ProcModel.h"

using namespace ProcGen;

ProcModel::ProcModel(Context* context) : LogicComponent(context) {
    SetUpdateEventMask(USE_NO_EVENT);
}

void ProcModel::CalculateNormals() {
    // works only when len(indices) == len(vertices)
    normals_.Clear();
    for (int i = 0; i < indices_.Size(); i += 3) {
        Vector3 edge1 = positions_[indices_[i+1]] - positions_[indices_[i]];
        Vector3 edge2 = positions_[indices_[i+2]] - positions_[indices_[i]];
        Vector3 normal = edge1.CrossProduct(edge2).Normalized();
        normals_.Push(normal); normals_.Push(normal); normals_.Push(normal);
    }
}

void ProcModel::Generate() {
    // vertex buffer
    if (!normals_.Size()) CalculateNormals();

    PODVector<VertexElement> vertexElements{{TYPE_VECTOR3, SEM_POSITION}};
    if (normals_.Size()) vertexElements.Push({TYPE_VECTOR3, SEM_NORMAL});
    if (colors_.Size()) vertexElements.Push({TYPE_VECTOR4, SEM_COLOR});
    if (uvs_.Size()) vertexElements.Push({TYPE_VECTOR2, SEM_TEXCOORD});

    VectorBuffer vecBuffer;
    for (int i = 0; i < positions_.Size(); i++) {
        if (positions_.Size()) vecBuffer.WriteVector3(positions_[i]);
        if (normals_.Size()) vecBuffer.WriteVector3(normals_[i]);
        if (colors_.Size()) vecBuffer.WriteColor(colors_[i]);
        if (uvs_.Size()) vecBuffer.WriteVector2(uvs_[i]);
    }

    SharedPtr<VertexBuffer>vertexBuffer(new VertexBuffer(context_));
    vertexBuffer->SetSize(positions_.Size(), vertexElements);
    vertexBuffer->SetData((void*)vecBuffer.GetData());
    vertexBuffer->SetShadowed(true);
    
    // index buffer
    SharedPtr<IndexBuffer>indexBuffer(new IndexBuffer(context_));
    indexBuffer->SetSize(indices_.Size(), false);
    indexBuffer->SetData(indices_.Buffer());
    indexBuffer->SetShadowed(true);

    // geometry
    Geometry* geometry = new Geometry(context_);
    geometry->SetNumVertexBuffers(1);
    geometry->SetVertexBuffer(0, vertexBuffer);
    geometry->SetIndexBuffer(indexBuffer);
    geometry->SetDrawRange(primitiveType_, 0, indexBuffer->GetIndexCount());

    // model
    model_ = new Model(context_);
    model_->SetNumGeometries(1);
    model_->SetGeometry(0, 0, geometry);
    
    // TODO
    // Optional. Morph ranges could also be not defined. Define a zero range (no morphing) for the vertex buffer.
    PODVector<unsigned> morphRangeStarts;
    PODVector<unsigned> morphRangeCounts;
    morphRangeStarts.Push(0);
    morphRangeCounts.Push(0);
    model_->SetVertexBuffers({vertexBuffer}, morphRangeStarts, morphRangeCounts);
    model_->SetIndexBuffers({indexBuffer});

    auto* staticModel = node_->GetComponent<StaticModel>();
    if (!staticModel) staticModel = node_->CreateComponent<StaticModel>();
    staticModel->SetModel(model_);
    if (material_.NotNull()) staticModel->SetMaterial(material_);
    
    // TODO
    // model->SetBoundingBox(BoundingBox(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f)));
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

bool ProcModel::GetDrawNormals() {
    return drawNormals_;
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
