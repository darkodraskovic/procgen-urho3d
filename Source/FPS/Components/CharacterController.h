//
// Copyright (c) 2008-2020 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include <Urho3D/Input/Controls.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

namespace FPS {
    
    const float MOVE_FORCE = 1.2f;
    const float INAIR_MOVE_FORCE = 0.02f;
    const float BRAKE_FORCE = 0.2f;
    const float JUMP_FORCE = 7.0f;
    const float INAIR_THRESHOLD_TIME = 0.1f;

/// Character component, responsible for physical movement according to controls, as well as animation.
    class CharacterController : public LogicComponent
    {
        URHO3D_OBJECT(CharacterController, LogicComponent);

    public:
        /// Construct.
        explicit CharacterController(Context* context);

        /// Register object factory and attributes.
        static void RegisterObject(Context* context);

        void Start() override;
        void Update(float timeStep) override;
        void FixedUpdate(float timeStep) override;
        void PostUpdate(float timeStep) override;
        
        // Referenced by ControllerManager
        Urho3D::Controls controls_;

    private:
        /// Handle physics collision event.
        void HandleNodeCollision(StringHash eventType, VariantMap& eventData);

        /// Grounded flag for movement.
        bool onGround_;
        /// Jump flag.
        bool okToJump_;
        /// In air timer. Due to possible physics inaccuracy, character can be off ground for max. 1/10 second and still be allowed to move.
        float inAirTimer_;
    };
}  // Voxels
