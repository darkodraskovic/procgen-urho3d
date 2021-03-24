#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/DebugRenderer.h>

using namespace Urho3D;

namespace ProcGen {

    class SceneManager : public Object {
        URHO3D_OBJECT(SceneManager, Object);

    public:
        explicit SceneManager (Context* context);
        void CreateScene();
        void CreateSkybox(const String& material);
        void SetupViewport();

        Scene* GetScene();

    private:
        SharedPtr<DebugRenderer> debugRenderer_;
        SharedPtr<Scene> scene_;
    };
}
