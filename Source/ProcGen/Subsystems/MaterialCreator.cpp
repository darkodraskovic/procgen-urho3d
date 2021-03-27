#include <Urho3D/Container/HashMap.h>
#include <Urho3D/Graphics/GraphicsDefs.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Graphics/Texture.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Math/Color.h>
#include <Urho3D/Math/Vector3.h>

#include "MaterialCreator.h"

using namespace ProcGen;

const HashMap<TextureUnit, String> textureDefines = {
    {TU_DIFFUSE, "DIFFMAP"}, {TU_NORMAL, "NORMALMAP"},
    {TU_SPECULAR, "SPECMAP"}, {TU_EMISSIVE, "EMISSIVEMAP"},
    {TU_ENVIRONMENT, "ENVCUBEMAP"},
};

MaterialCreator::MaterialCreator(Context* context) : Object(context) {}

void MaterialCreator::Start() {
    cache_ = GetSubsystem<ResourceCache>();
}

Material* MaterialCreator::Create(Technique* technique, const HashMap<TextureUnit, Texture*>& textureData) {
    Material* material(new Material(context_));
    material->SetTechnique(0, technique);
    for (TextureUnit tu : textureData.Keys()) {
        material->SetTexture(tu, *textureData[tu]);
    }
    return material;
}

Material* MaterialCreator::Create(const String& shader, const Color& color, const HashMap<TextureUnit, Texture*>& textureData) {
    // Technique
    Technique *technique(new Technique(context_));
    Pass* pass = technique->CreatePass("base");
    pass->SetVertexShader(shader);
    pass->SetPixelShader(shader);
    
    String defines;
    for (TextureUnit tu : textureData.Keys()) {
        defines +=* textureDefines[tu] + " ";
    }
    if (defines != "") {
        pass->SetVertexShaderDefines(defines);
        pass->SetPixelShaderDefines(defines);
    }

    // Material
    Material* material = Create(technique, textureData);
    material->SetShaderParameter("MatDiffColor", color);
    
    return material;
}
