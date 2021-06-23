#include <Urho3D/Graphics/GraphicsDefs.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Math/Rect.h>
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

    CreateCamera();
}

 void TextureCreator::SetScene(Scene *scene) {
    scene_ = scene;

    auto* octree = scene_->CreateComponent<Octree>();
    if (!octree) scene_->CreateComponent<Octree>();

    cameraNode_ =  scene_->GetChild("Camera");
    if (!cameraNode_) CreateCamera();
}

void TextureCreator::CreateCamera() {
    // Create a camera for the render-to-texture scene
    // Leave it at the world origin and let it observe the scene
    cameraNode_ =  scene_->CreateChild("Camera");
    auto* camera = cameraNode_->CreateComponent<Camera>();
    camera->SetFarClip(100.0f);
}

Texture2D* TextureCreator::CreateImageTexture(Image* image) {
    Texture2D* texture2D(new Texture2D(context_));
    texture2D->SetSize(image->GetWidth(), image->GetHeight(), Graphics::GetRGBFormat(), Urho3D::TEXTURE_DYNAMIC);
    texture2D->SetData(image);
    return texture2D;
}

Texture2D* TextureCreator::CreateRenderTexture(int w, int h, RenderSurfaceUpdateMode mode) {
    Texture2D* renderTexture = new Texture2D(context_);
    renderTexture->SetSize(w, h, Graphics::GetRGBFormat(), TEXTURE_RENDERTARGET);
    // renderTexture->SetFilterMode(FILTER_BILINEAR);

    // Texture's RenderSurface object exists when the texture has been created in rendertarget mode
    RenderSurface* surface = renderTexture->GetRenderSurface();
    surface->SetUpdateMode(mode);
    if (mode == Urho3D::SURFACE_MANUALUPDATE) surface->QueueUpdate();

    // Define the viewport for rendering the auxiliary scene, similarly as how backbuffer viewports are defined
    // to the Renderer subsystem. The viewport will be updated when the render texture is visible in the main view
    Viewport* viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    surface->SetViewport(0, viewport);

    return renderTexture;
}

Texture2D* TextureCreator::CreateEffectTexture(int w, int h, const String& shader, RenderSurfaceUpdateMode mode) {
    RenderPathCommand rpCommand;
    // Render a viewport-sized quad using the specified shaders
    rpCommand.type_ = RenderCommandType::CMD_QUAD;
    rpCommand.SetOutput(0, "viewport"); // scene -> camera -> viewport -> surface -> texture
    rpCommand.vertexShaderName_ = shader;
    rpCommand.pixelShaderName_ = shader;

    // Default RenderPath is Forward and it contains clear, scenepasses and forwardlights commands.
    // Since we only need one quad command, we make a new RenderPath
    RenderPath* renderPath = new RenderPath();
    renderPath->AddCommand(rpCommand);
    
    Texture2D* renderTexture = CreateRenderTexture(w, h, mode);
    auto* viewport = renderTexture->GetRenderSurface()->GetViewport(0);
    viewport->SetRenderPath(renderPath);

    return renderTexture;
}
