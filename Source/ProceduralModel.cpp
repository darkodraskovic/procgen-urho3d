#include <iostream>
#include "ProceduralModel.h"

#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/GraphicsDefs.h>
#include <Urho3D/Graphics/IndexBuffer.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
  
ProceduralModel::ProceduralModel(Context *context) : Object(context){
    model_ = new Model(context_);

    positions_ = new VertexBuffer(context_);
    // Optionally a shadow copy of the vertex data can exist in CPU memory,
    // to allow e.g. raycasts into the geometry without having to lock and read GPU memory.
    positions_->SetShadowed(true);    
    normals_ = new VertexBuffer(context_);
    normals_->SetShadowed(true);    
    indices_ = new IndexBuffer(context_);
    indices_->SetShadowed(true);
    
    geometry_ = new Geometry(context_);
    model_->SetNumGeometries(1);
    model_->SetGeometry(0, 0, geometry_);
};

void ProceduralModel::SetPositions(float* data, unsigned numVertices) {
    positions_->SetSize(numVertices, MASK_POSITION);
    positions_->SetData(data);
}

void ProceduralModel::SetNormals(float* data){
    normals_->SetSize(positions_->GetVertexCount(), MASK_NORMAL);
    normals_->SetData(data);
}

void ProceduralModel::SetIndices(unsigned short* data, unsigned numIndices){
    if (!numIndices) numIndices = positions_->GetVertexCount();
    indices_->SetSize(numIndices, false);
    indices_->SetData(data);
}

void ProceduralModel::Generate() {
    geometry_->SetNumVertexBuffers(2);
    geometry_->SetVertexBuffer(0, positions_);
    geometry_->SetVertexBuffer(1, normals_);
    geometry_->SetIndexBuffer(indices_);
    geometry_->SetDrawRange(TRIANGLE_LIST, 0, indices_->GetIndexCount());

    // Optional. The vertex & index buffers must be listed in the model so that it can be saved properly.
    vertexBuffers_.Push(positions_);
    vertexBuffers_.Push(normals_);
    indexBuffers_.Push(indices_);
    // TODO
    // Optional. Morph ranges could also be not defined. Define a zero range (no morphing) for the vertex buffer.
    PODVector<unsigned> morphRangeStarts;
    PODVector<unsigned> morphRangeCounts;
    morphRangeStarts.Push(0);
    morphRangeCounts.Push(0);
    model_->SetVertexBuffers(vertexBuffers_, morphRangeStarts, morphRangeCounts);
    model_->SetIndexBuffers(indexBuffers_);

    // TODO
    // model_->SetBoundingBox(BoundingBox(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f)));
}
