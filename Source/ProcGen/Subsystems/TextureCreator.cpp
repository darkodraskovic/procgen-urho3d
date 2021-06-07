#include <Urho3D/Graphics/GraphicsDefs.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/RenderPath.h>
#include <Urho3D/Resource/XMLFile.h>

#include "TextureCreator.h"

using namespace ProcGen;

TextureCreator::TextureCreator(Context* context) : Object(context) {}

void TextureCreator::Start() {
        // Create the scene which will be rendered to a texture
        scene_ = new Scene(context_);
        // Create octree, use default volume (-1000, -1000, -1000) to (1000, 1000, 1000)
        scene_->CreateComponent<Octree>();

        // Create a camera for the render-to-texture scene
        // Leave it at the world origin and let it observe the scene
        cameraNode_ =  scene_->CreateChild("Camera");
        auto* camera = cameraNode_->CreateComponent<Camera>();
        camera->SetFarClip(100.0f);

        // Create a point light to the camera scene node
        // auto* light = cameraNode_->CreateComponent<Light>();
        // light->SetLightType(LIGHT_POINT);
        // light->SetRange(30.0f);
}

Texture2D* TextureCreator::CreateImageTexture(Image* image) {
    Texture2D* texture2D(new Texture2D(context_));
    texture2D->SetSize(image->GetWidth(), image->GetHeight(), Graphics::GetRGBFormat(), Urho3D::TEXTURE_DYNAMIC);
    texture2D->SetData(image);
    return texture2D;
}

Texture2D* TextureCreator::CreateEffectTexture(int w, int h, const String& shader, RenderSurfaceUpdateMode mode) {
    auto* cache = GetSubsystem<ResourceCache>();

    Texture2D* renderTexture = new Texture2D(context_);
    renderTexture->SetSize(w, h, Graphics::GetRGBFormat(), TEXTURE_RENDERTARGET);
    renderTexture->SetFilterMode(FILTER_BILINEAR);

    // Get the texture's RenderSurface object (exists when the texture has been created in rendertarget mode)
    RenderSurface* surface = renderTexture->GetRenderSurface();
    // and define the viewport for rendering the auxiliary scene, similarly as how backbuffer viewports are defined
    // to the Renderer subsystem. The viewport will be updated when the texture is visible in the main view
    Viewport* viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    
    surface->SetUpdateMode(mode);
    if (mode == Urho3D::SURFACE_MANUALUPDATE) surface->QueueUpdate();
    surface->SetViewport(0, viewport);

    RenderPathCommand rpCommand;
    rpCommand.SetOutput(0, "viewport");
    rpCommand.type_ = RenderCommandType::CMD_QUAD;
    rpCommand.vertexShaderName_ = shader;
    rpCommand.pixelShaderName_ = shader;
    rpCommand.SetTextureName(Urho3D::TU_DIFFUSE, "viewport");

    // RenderPath* renderPath = new RenderPath();
    SharedPtr<RenderPath> renderPath = viewport->GetRenderPath()->Clone();
    renderPath->AddCommand(rpCommand);
    viewport->SetRenderPath(renderPath);

    return renderTexture;
}
