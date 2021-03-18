#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Math/Color.h>

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
        pass->SetPixelShaderDefines("DIFFMAP");
    }

    return material;
}
