#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Math/Color.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/TextureCube.h>

using namespace Urho3D;

namespace ProcGen {

    class MaterialCreator : public Object {
        URHO3D_OBJECT(MaterialCreator, Object);

    public:
        explicit MaterialCreator (Context* context);
        void Start();

        Material* Create(Technique* technique, const HashMap<TextureUnit, Texture*>& textureData);
        Material* Create(const String& shader, const Color& color = Color::WHITE,
                         const HashMap<TextureUnit, Texture*>& textureData = HashMap<TextureUnit, Texture*>{});

    private:
        ResourceCache* cache_;
    };

}
