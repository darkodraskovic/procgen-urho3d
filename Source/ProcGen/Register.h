#include "Subsystems/GeometryCreator.h"
#include "Subsystems/SceneManager.h"
#include "Subsystems/ModelCreator.h"
#include "Subsystems/TextureCreator.h"
#include "Subsystems/MaterialCreator.h"
#include "Subsystems/Controller.h"

#include "Components/CameraController.h"
#include "Components/ProcModel.h"

namespace ProcGen {
    inline void Register(Context* context) {
        context->RegisterSubsystem<ProcGen::SceneManager>();
        context->RegisterSubsystem<ProcGen::ModelCreator>();
        context->RegisterSubsystem<ProcGen::TextureCreator>();
        context->RegisterSubsystem<ProcGen::MaterialCreator>();
        context->RegisterSubsystem<ProcGen::GeometryCreator>();
        context->RegisterSubsystem<ProcGen::Controller>();

        context->RegisterFactory<ProcGen::CameraController>();
        context->RegisterFactory<ProcGen::ProcModel>();
    }
}
