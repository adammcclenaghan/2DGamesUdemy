#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "./AssetManager.h"
#include "./Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "../lib/glm/glm.hpp"

EntityManager manager;
SDL_Renderer* Game::renderer;
AssetManager* Game::assetManager = new AssetManager(&manager);

Game::Game() {
  this->isRunning = false;
}

Game::~Game() {
}

bool Game::IsRunning() const {

  return this->isRunning;
}

void Game::Initialize(int width, int height)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    std::cerr << "Error initializing SDL." << std::endl;
    return;
  }

  // Create a window
  window = SDL_CreateWindow(NULL,
			    SDL_WINDOWPOS_CENTERED,
			    SDL_WINDOWPOS_CENTERED,
			    width,
			    height,
			    SDL_WINDOW_BORDERLESS);

  if (!window)
  {
    std::cerr << "Error creating SDL window" << std::endl;
    return;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer)
  {
    std::cerr << "Error creating SDL renderer." << std::endl;
    return;
  }

  LoadLevel(0);

  isRunning = true;
  return;
}

void Game::LoadLevel(int levelNumber) {
  // Start including new assets to the assetmanager list
  std::string textureFilePath = "./assets/images/tank-big-right.png";
  assetManager->AddTexture("tank-image", textureFilePath.c_str());
  
  // Start including entities and also components to them
  
  
  //TODO: add entities and add components to the entities
  Entity& newEntity(manager.AddEntity("tank"));
  newEntity.AddComponent<TransformComponent>(0, 0, 20, 20, 32, 32, 1);
  newEntity.AddComponent<SpriteComponent>("tank-image");
  
  Entity& anotherEntity(manager.AddEntity("projectile2"));
  anotherEntity.AddComponent<TransformComponent>(50, 50, 20, 20, 32, 32, 1);
  anotherEntity.AddComponent<SpriteComponent>("tank-image");
  //List all entities and components for the level in cout
  manager.ListAllEntities();
}

void Game::ProcessInput() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch(event.type) {

  case SDL_QUIT: {
    isRunning = false;
    break;
  }
  case SDL_KEYDOWN : {
    if (event.key.keysym.sym == SDLK_ESCAPE) {
      isRunning = false;
    }
  }
  default: {
    break;
  } 
  }
}

void Game::Update() {
  // Delta time is the difference in ticks from last frame converted to seconds
  unsigned int currentTicks = SDL_GetTicks();
  
  // Wait until our target time has elapsed since the last frame
  int timeToWait = FRAME_TARGET_TIME_MS - (SDL_GetTicks() - ticksLastFrame);
  // Only call delay if we are too fast to process this frame
  if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME_MS) {
    SDL_Delay(timeToWait);
  }
  
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME_MS));
  
  // sets the new ticks for the current frame to be used in the next pass - IN SECONDS
  float deltaTime = (currentTicks - ticksLastFrame) / 1000.0f;

  // Clamp the deltaTime to a maximum value - this is useful for example when
  // we are debugging where the delta time might grow to a very large value when
  // we are sitting at a breakpoint

  deltaTime = (deltaTime > MAX_DELTA_TIME) ? MAX_DELTA_TIME : deltaTime;
  
  ticksLastFrame = currentTicks;

  manager.Update(deltaTime);
}

void Game::Render() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  if (manager.HasNoEntities()) {
    return;
  }
  
  manager.Render();
  SDL_RenderPresent(renderer);
}

void Game::Destroy() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}


