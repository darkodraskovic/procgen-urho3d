#pragma once

#include <Urho3D/Urho3DAll.h>

namespace ProcGen {

class MaterialCreator : public Object {
  URHO3D_OBJECT(MaterialCreator, Object);

 public:
  explicit MaterialCreator(Context* context);

  Material* Create(const String& technique,
                   const HashMap<TextureUnit, Texture*>& textureData =
                       HashMap<TextureUnit, Texture*>{});
};
}  // namespace ProcGen
