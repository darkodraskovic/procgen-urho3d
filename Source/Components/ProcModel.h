#pragma once

#include <Urho3D/Container/Vector.h>
#include <Urho3D/Graphics/GraphicsDefs.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Math/Vector2.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/Graphics/IndexBuffer.h>
#include <Urho3D/Math/Vector3.h>

using namespace Urho3D;

namespace ProcGen {

    class ProcModel : public LogicComponent
    {
        URHO3D_OBJECT(ProcModel, LogicComponent)
        public:
        explicit ProcModel(Context* context);

        // void Update(float timeStep) override;
        // virtual void PostUpdate (float timeStep) override;
        void HandlePostRenderUpdate (StringHash eventType, VariantMap& eventData);
        void CalculateNormals();
        void Generate();
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
        
    protected:
        SharedPtr<Model> model_;

        bool drawNormals_ = false;
    };

}  // ProcGen
