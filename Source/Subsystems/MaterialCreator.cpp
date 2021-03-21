#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Graphics/Texture.h>
#include <Urho3D/Graphics/TextureCube.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Math/Color.h>
#include <Urho3D/Math/Vector3.h>

#include "MaterialCreator.h"

using namespace ProcGen;

MaterialCreator::MaterialCreator(Context* context) : Object(context) {}

void MaterialCreator::Start() {
    cache_ = GetSubsystem<ResourceCache>();
}

Material* MaterialCreator::Create(const String& shader, const Color& color, Texture* diffuse) {
    Material* material(new Material(context_));
    
    Technique *technique(new Technique(context_));
    Pass* pass = technique->CreatePass("base");
    pass->SetVertexShader(shader);
    pass->SetPixelShader(shader);
    material->SetTechnique(0, technique);
    
    material->SetShaderParameter("MatDiffColor", color);
    
    if (diffuse) {
        material->SetTexture(TU_DIFFUSE, diffuse);
        pass->SetVertexShaderDefines("DIFFMAP");
        pass->SetPixelShaderDefines("DIFFMAP");
    }

    auto* envTexture = cache_->GetResource<TextureCube>("Textures/Skybox.xml");
    material->SetTexture(Urho3D::TU_ENVIRONMENT, envTexture);
    material->SetShaderParameter("MatEnvMapColor", Color::WHITE);

    return material;
}
