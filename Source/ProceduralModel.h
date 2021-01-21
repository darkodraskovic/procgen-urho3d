#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/Graphics/Model.h>

using namespace Urho3D;

class ProceduralModel : public Object
{
    URHO3D_OBJECT(ProceduralModel, Object)
public:
    explicit ProceduralModel(Context* context);

    void SetPositions(float* vertexData, unsigned numVertices);
    void SetNormals(float* vertexData);
    void SetIndices(unsigned short* indexData, unsigned numVertices = 0);
    void Generate();
    
    SharedPtr<Model> model_;
private:
    
    Vector<SharedPtr<VertexBuffer> > vertexBuffers_;
    Vector<SharedPtr<IndexBuffer> > indexBuffers_;    
    SharedPtr<VertexBuffer> positions_;
    SharedPtr<VertexBuffer> normals_;
    SharedPtr<IndexBuffer> indices_;
    
    SharedPtr<Geometry> geometry_;
};
