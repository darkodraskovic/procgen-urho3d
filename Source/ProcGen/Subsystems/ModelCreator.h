#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Scene/Node.h>

using namespace Urho3D;

namespace ProcGen {

    class ModelCreator : public Object {
        URHO3D_OBJECT(ModelCreator, Object);

    public:
        explicit ModelCreator (Context* context);
        void Start();
        Node* CreateStockModel(const String& modelName, Material* material = nullptr);

    private:
        SharedPtr<Material> defaultMaterial_;
    };
}
