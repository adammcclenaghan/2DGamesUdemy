#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "./AssetManager.h"
#include "./Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/KeyboardControlComponent.h"
#include "./Components/ColliderComponent.h"
#include "./Components/TextLabelComponent.h"
#include "./Components/ProjectileEmitterComponent.h"
#include "./Map.h"
#include "../lib/glm/glm.hpp"

EntityManager manager;
SDL_Renderer* Game::renderer;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Map* map;

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

  if (TTF_Init() != 0) {
    std::cerr << "Error initializing SDL TTF" << std::endl;
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

Entity& player(manager.AddEntity("chopper", PLAYER_LAYER));

void Game::LoadLevel(int levelNumber) {
  // Start including new assets to the assetmanager list
  assetManager->AddTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
  assetManager->AddTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
  assetManager->AddTexture("radar-image", std::string("./assets/images/radar.png").c_str());
  assetManager->AddTexture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());
  assetManager->AddTexture("heliport-image", std::string("./assets/images/heliport.png").c_str());
  assetManager->AddFont("charriot-font", std::string("./assets/fonts/charriot.ttf").c_str(), 14);
  assetManager->AddTexture("projectile-image", std::string("./assets/images/bullet-enemy.png").c_str());
			  
  map = new Map("jungle-tiletexture", 2, 32);
  //The 25, 20 here is defined by the size of columns/rows in jungle.map
  //TODO: Think it is possible to work this out while reading over the jungle.map
  // on load and instead set it that way. Might require multiple reads of the file..
  // so maybe read once and store it all in memory if a second pass would be required
  map->LoadMap("./assets/tilemaps/jungle.map", 25, 20);
      
  player.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
  player.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
  player.AddComponent<KeyboardControlComponent>("e", "f", "d", "s", "space");
  // It doesn't really matter what we initialize with here as the update will sort it on its first call based on the TransformComponent values
  player.AddComponent<ColliderComponent>("PLAYER", 240, 106, 32, 32);
  
  //TODO: add entities and add components to the entities
  Entity& tankEntity(manager.AddEntity("tank", ENEMY_LAYER));
  tankEntity.AddComponent<TransformComponent>(150, 495, 0, 0, 32, 32, 1);
  tankEntity.AddComponent<SpriteComponent>("tank-image");
  tankEntity.AddComponent<ColliderComponent>("ENEMY", 150, 495, 32, 32);

  Entity& projectile(manager.AddEntity("projectile", PROJECTILES));
  projectile.AddComponent<TransformComponent>(150+16, 495+16, 0, 0, 4, 4, 1);
  projectile.AddComponent<SpriteComponent>("projectile-image");
  projectile.AddComponent<ColliderComponent>("PROJECTILE", 150+16, 495+16, 4, 4);
  projectile.AddComponent<ProjectileEmitterComponent>(50, 270, 200, true);

  Entity& heliport(manager.AddEntity("Heliport", OBSTACLE_LAYER));
  heliport.AddComponent<TransformComponent>(470, 420, 0, 0, 32, 32, 1);
  heliport.AddComponent<SpriteComponent>("heliport-image");
  heliport.AddComponent<ColliderComponent>("LEVEL_COMPLETE", 470, 420, 32, 32);
  
  Entity& radarEntity(manager.AddEntity("Radar", UI_LAYER));
  radarEntity.AddComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
  radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);

  Entity& labelLevelName(manager.AddEntity("LabelLevelName", UI_LAYER));
  labelLevelName.AddComponent<TextLabelComponent>(10, 10, "First level...", "charriot-font", WHITE_COLOR);

  //List all entities and components for the level in cout
  manager.ListAllEntities();
}

void Game::ProcessInput() {
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

  HandleCameraMovement();

  CheckCollisions();
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

void Game::HandleCameraMovement() {
  TransformComponent* mainPlayerTransform = player.GetComponent<TransformComponent>();
  
  // Follow the player
  camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
  camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

  // Clamp to min and max bounds
  camera.x = camera.x < 0 ? 0 : camera.x;
  camera.y = camera.y < 0 ? 0 : camera.y;
  camera.x = camera.x > camera.w ? camera.w : camera.x;
  camera.y = camera.y > camera.h ? camera.h : camera.y;
}

void Game::CheckCollisions() {
  CollisionType collisionType = manager.CheckCollisions();
  if (collisionType == PLAYER_ENEMY_COLLISION) {
    ProcessGameOver();
  }
  if (collisionType == PLAYER_PROJECTILE_COLLISION) {
    ProcessGameOver();
  }
  else if(collisionType == PLAYER_LEVEL_COMPLETE_COLLISION) {
    ProcessNextLevel(1);
  }
}

void Game::ProcessNextLevel(int levelNumber) {
  std::cout << "Next level: " << std::endl;
  isRunning = false;
}

void Game::ProcessGameOver() {
  std::cout << "Game over" << std::endl;
  isRunning = false;
}
void Game::Destroy() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
