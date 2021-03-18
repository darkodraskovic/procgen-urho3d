#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Math/Color.h>
#include <Urho3D/Resource/ResourceCache.h>

using namespace Urho3D;

namespace ProcGen {

    class MaterialCreator : public Object {
        URHO3D_OBJECT(MaterialCreator, Object);

    public:
        explicit MaterialCreator (Context* context);
        void Start();

        Material* Create(const String& shader, const Color& color = Color::WHITE, Texture* diffuse = nullptr );

    private:
        ResourceCache* cache_;
    };

}
