#pragma once

#include <Urho3D/Container/HashMap.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Graphics/GraphicsDefs.h>
#include <Urho3D/Graphics/Texture.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Scene/Scene.h>

using namespace Urho3D;

namespace ProcGen {
    class TextureCreator : public Object {
        URHO3D_OBJECT(TextureCreator, Object);

    public:
        explicit TextureCreator (Context* context);
        
        void Start();
        Texture2D* CreateImageTexture(Image* image);
        Texture2D* CreateRenderTexture(int w, int h, RenderSurfaceUpdateMode mode = SURFACE_UPDATEVISIBLE);
        Texture2D* CreateEffectTexture(int w, int h, const String& shader, RenderSurfaceUpdateMode mode = SURFACE_UPDATEVISIBLE);

        void SetScene(Scene* scene);

    private:
        void CreateCamera();
        
        SharedPtr<Scene> scene_;
        Node* cameraNode_;
    };
}
