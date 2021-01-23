#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/Graphics/IndexBuffer.h>
#include <Urho3D/Graphics/Model.h>

using namespace Urho3D;

namespace ProcGen {

    class ModelGenerator : public Object
    {
        URHO3D_OBJECT(ModelGenerator, Object)
        public:
        explicit ModelGenerator(Context* context);

        void SetVertexBuffer(void* vertexData, VertexMask semantics, unsigned numVertices = 0);
        void SetIndices(unsigned short* indexData, unsigned numIndices = 0);
        Model* Generate();
        void Reset();

    private:
        Vector<SharedPtr<VertexBuffer>> vertexBuffers_;
        Vector<SharedPtr<IndexBuffer>> indexBuffers_;
    };

}  // ProcGen
