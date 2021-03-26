#include "Chunk.h"

using namespace ProcGen;

Chunk::Chunk(Context* context) : LogicComponent(context) {
    SetUpdateEventMask(USE_NO_EVENT);
  }

void Chunk::Start() {

}

void Chunk::Update(float timeStep) {

}
