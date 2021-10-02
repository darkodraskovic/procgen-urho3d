#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Graphics/Texture.h>
#include <Urho3D/Resource/ResourceCache.h>

#include "MaterialCreator.h"

using namespace ProcGen;

MaterialCreator::MaterialCreator(Context* context) : Object(context) {}

Material* MaterialCreator::Create(const String& technique, const HashMap<TextureUnit, Texture*>& textureData) {
    auto tech = GetSubsystem<ResourceCache>()->GetResource<Technique>("CoreData/Techniques/" + technique +".xml");
    
    Material* material(new Material(context_));
    material->SetTechnique(0, tech);
    for (TextureUnit tu : textureData.Keys()) {
        material->SetTexture(tu, *textureData[tu]);
    }
    
    return material;
}
