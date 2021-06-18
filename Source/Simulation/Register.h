#include "Vehicle.h"

namespace Simulation {
    inline void Register(Context* context) {
        context->RegisterSubsystem<Simulation::Vehicle>();
    }
}
