#include "App.h"

URHO3D_DEFINE_APPLICATION_MAIN(App)

#include "FPS/Engine.h"
#include "Maze/Engine.h"
#include "ProcGen/Engine.h"
#include "Simulation/Engine.h"
#include "Toys/ProcModelToy.h"
#include "Toys/ShaderToy.h"
#include "Toys/SimulationToy.h"
#include "Toys/VoxelToy.h"
#include "Voxels/Engine.h"

App::App(Context* context) : Application(context) {
  context_->RegisterSubsystem<ProcGen::Engine>();
  context_->RegisterSubsystem<Simulation::Engine>();
  context_->RegisterSubsystem<Maze::Engine>();
  context_->RegisterSubsystem<Voxels::Engine>();
  context_->RegisterSubsystem<FPS::Engine>();

  context_->RegisterSubsystem<Toy::ShaderToy>();
  context_->RegisterSubsystem<Toy::VoxelToy>();
  context_->RegisterSubsystem<Toy::SimulationToy>();
  context_->RegisterSubsystem<Toy::ProcModelToy>();
}

void App::Setup() {
  engineParameters_[EP_FULL_SCREEN] = false;
  engineParameters_[EP_WINDOW_POSITION_X] = 0;
  engineParameters_[EP_WINDOW_POSITION_Y] = 0;
  engineParameters_[EP_WINDOW_WIDTH] = 1366;
  engineParameters_[EP_WINDOW_HEIGHT] = 768;
  engineParameters_[EP_MULTI_SAMPLE] = 4;
}

void App::Start() {
  CreateConsoleAndDebugHud();
  SubscribeToEvents();

  // ================================================================
  // CORE
  GetSubsystem<ProcGen::Engine>()->Start();
  GetSubsystem<Simulation::Engine>()->Start();
  GetSubsystem<Maze::Engine>()->Start();
  GetSubsystem<Voxels::Engine>()->Start();
  GetSubsystem<FPS::Engine>()->Start();

  // Scene addenda
  auto* sceneManager = GetSubsystem<FPS::SceneManager>();
  sceneManager->SetupLights();
  sceneManager->CreateSkybox("Materials/Space.xml");
  sceneManager->SetupEffects();

  // ================================================================
  // TOYS

  // SHADER
  // GetSubsystem<Toy::ShaderToy>()->Start();

  // VOXELS
  // GetSubsystem<Toy::VoxelToy>()->Start();

  // PROC MODEL
  // GetSubsystem<Toy::ProcModelToy>()->Start();

  // CUSTOM GEOM
  GetSubsystem<Toy::SimulationToy>()->Start();
}

void App::CreateConsoleAndDebugHud() {
  // Get default style
  ResourceCache* cache = GetSubsystem<ResourceCache>();
  XMLFile* xmlFile = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");

  // Create console
  Console* console = engine_->CreateConsole();
  console->SetDefaultStyle(xmlFile);
  console->GetBackground()->SetOpacity(0.8f);

  // Create debug HUD.
  DebugHud* debugHud = engine_->CreateDebugHud();
  debugHud->SetDefaultStyle(xmlFile);
}

void App::Stop() {}

void App::SubscribeToEvents() {
  SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(App, HandleKeyDown));
  SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(App, HandleUpdate));
  SubscribeToEvent(E_POSTUPDATE, URHO3D_HANDLER(App, HandlePostUpdate));
  // SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(App,
  // HandlePostrenderupdate));
}

void App::HandleKeyDown(StringHash eventType, VariantMap& eventData) {
  using namespace KeyDown;
  int key = eventData[P_KEY].GetInt();
  if (key == KEY_ESCAPE) engine_->Exit();

  // Toggle console with F1
  if (key == KEY_F1) GetSubsystem<Console>()->Toggle();

  // Toggle debug HUD with F2
  else if (key == KEY_F2)
    GetSubsystem<DebugHud>()->ToggleAll();
}

void App::HandleUpdate(StringHash eventType, VariantMap& eventData) {}

void App::HandlePostUpdate(StringHash eventType, VariantMap& eventData) {}
