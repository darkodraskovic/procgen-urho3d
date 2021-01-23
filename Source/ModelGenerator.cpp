#include <iostream>

#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/StaticModel.h>

#include "ModelGenerator.h"

using namespace ProcGen;

ModelGenerator::ModelGenerator(Context *context) : Object(context){};

void ModelGenerator::SetVertexBuffer(void* data, VertexMask semantics, unsigned numVertices) {
    if (!numVertices) numVertices = vertexBuffers_.At(0)->GetVertexCount();
    VertexBuffer* buffer = new VertexBuffer(context_);
    buffer->SetSize(numVertices, semantics);
    buffer->SetData(data);
    buffer->SetShadowed(true);
    vertexBuffers_.Push(SharedPtr<VertexBuffer>(buffer));
}

void ModelGenerator::SetIndices(unsigned short* data, unsigned numIndices){
    if (!numIndices) numIndices = vertexBuffers_.At(0)->GetVertexCount();
    IndexBuffer* indices = new IndexBuffer(context_);
    indices->SetSize(numIndices, false);
    indices->SetData(data);
    indices->SetShadowed(true);
    indexBuffers_.Push(SharedPtr<IndexBuffer>(indices));
}

Model* ModelGenerator::Generate() {
    Model* model = new Model(context_);

    Geometry* geometry = new Geometry(context_);
    model->SetNumGeometries(1);
    model->SetGeometry(0, 0, geometry);

    geometry->SetNumVertexBuffers(vertexBuffers_.Size());
    for (int i = 0; i < vertexBuffers_.Size(); i++) {
        geometry->SetVertexBuffer(i, vertexBuffers_.At(i));
    }
    geometry->SetIndexBuffer(indexBuffers_.At(0));
    geometry->SetDrawRange(TRIANGLE_LIST, 0, indexBuffers_.At(0)->GetIndexCount());

    // TODO
    // Optional. Morph ranges could also be not defined. Define a zero range (no morphing) for the vertex buffer.
    PODVector<unsigned> morphRangeStarts;
    PODVector<unsigned> morphRangeCounts;
    morphRangeStarts.Push(0);
    morphRangeCounts.Push(0);
    model->SetVertexBuffers(vertexBuffers_, morphRangeStarts, morphRangeCounts);
    model->SetIndexBuffers(indexBuffers_);

    // TODO
    // model->SetBoundingBox(BoundingBox(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f)));

    return model;
}

void ModelGenerator::Reset() {
    vertexBuffers_.Clear();
    indexBuffers_.Clear();
}
