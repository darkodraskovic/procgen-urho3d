#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Graphics/Material.h>

using namespace Urho3D;

namespace ProcGen {

    class MaterialCreator : public Object {
        URHO3D_OBJECT(MaterialCreator, Object);

    public:
        explicit MaterialCreator (Context* context);

        Material* Create(const String& technique, const HashMap<TextureUnit, Texture*>& textureData = HashMap<TextureUnit, Texture*>{});
    };
}
