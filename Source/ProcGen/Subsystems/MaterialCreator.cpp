#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Graphics/Texture.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Math/Color.h>
#include <Urho3D/Math/Vector3.h>

#include "MaterialCreator.h"

using namespace ProcGen;

MaterialCreator::MaterialCreator(Context* context) : Object(context) {}

void MaterialCreator::Start() {
    cache_ = GetSubsystem<ResourceCache>();
}

Material* MaterialCreator::Create(const String& technique, const HashMap<TextureUnit, Texture*>& textureData) {
    auto techn = cache_->GetResource<Technique>("CoreData/Techniques/" + technique +".xml");
    
    Material* material(new Material(context_));
    material->SetTechnique(0, techn);
    for (TextureUnit tu : textureData.Keys()) {
        material->SetTexture(tu, *textureData[tu]);
    }
    
    return material;
}
